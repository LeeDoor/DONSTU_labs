import html
import json
import math
import re
from collections import defaultdict
from pathlib import Path

INPUT_FILE = Path("searches_benchmark.txt")
OUTPUT_FILE = Path("searches_benchmark.html")

def parse_time_to_ns(value):
    value = value.strip()
    match = re.fullmatch(r"([0-9]+(?:\.[0-9]+)?)(h|min|s|ms|us|ns)", value)
    if not match:
        return 0.0

    number = float(match.group(1))
    suffix = match.group(2)

    multipliers = {
        "h": 3_600_000_000_000,
        "min": 60_000_000_000,
        "s": 1_000_000_000,
        "ms": 1_000_000,
        "us": 1_000,
        "ns": 1,
    }

    return number * multipliers[suffix]

def parse_report(text):
    sections = []
    current_section = None
    header_seen = False

    for raw_line in text.splitlines():
        line = raw_line.strip()

        if not line:
            current_section = None
            header_seen = False
            continue

        parts = line.split()

        if current_section is None:
            current_section = {
                "name": line,
                "rows": [],
            }
            sections.append(current_section)
            header_seen = False
            continue

        if not header_seen:
            header_seen = True
            continue

        if len(parts) < 5:
            continue

        size = int(parts[0])
        algorithm = parts[1]
        scenario = parts[2]
        time_text = parts[3]
        comparisons = float(parts[4])

        current_section["rows"].append({
            "size": size,
            "algorithm": algorithm,
            "scenario": scenario,
            "time_text": time_text,
            "time_ns": parse_time_to_ns(time_text),
            "comparisons": comparisons,
        })

    return sections

def nice_number(value):
    if value == 0:
        return "0"
    if value >= 1_000_000_000:
        return f"{value / 1_000_000_000:.2f}s"
    if value >= 1_000_000:
        return f"{value / 1_000_000:.2f}ms"
    if value >= 1_000:
        return f"{value / 1_000:.2f}us"
    return f"{value:.2f}ns"

def format_number(value):
    if abs(value - round(value)) < 0.000001:
        return str(int(round(value)))
    return f"{value:.2f}"

def make_svg_chart(rows, metric, title, y_label):
    width = 920
    height = 380
    left = 70
    right = 30
    top = 40
    bottom = 55
    plot_width = width - left - right
    plot_height = height - top - bottom

    sizes = sorted({row["size"] for row in rows})
    series = defaultdict(list)

    for row in rows:
        key = f'{row["algorithm"]} / {row["scenario"]}'
        series[key].append((row["size"], row[metric]))

    values = [row[metric] for row in rows]
    y_max = max(values) if values else 1
    y_max = y_max if y_max > 0 else 1

    def x_pos(size):
        if len(sizes) == 1:
            return left + plot_width / 2
        index = sizes.index(size)
        return left + index * plot_width / (len(sizes) - 1)

    def y_pos(value):
        return top + plot_height - value * plot_height / y_max

    palette = [
        "#2563eb",
        "#dc2626",
        "#16a34a",
        "#9333ea",
        "#ea580c",
        "#0891b2",
        "#4f46e5",
        "#be123c",
        "#15803d",
        "#7c3aed",
    ]

    elements = []

    elements.append(f'<svg viewBox="0 0 {width} {height}" class="chart" role="img" aria-label="{html.escape(title)}">')
    elements.append(f'<text x="{left}" y="24" class="chart-title">{html.escape(title)}</text>')

    elements.append(f'<line x1="{left}" y1="{top}" x2="{left}" y2="{top + plot_height}" class="axis"/>')
    elements.append(f'<line x1="{left}" y1="{top + plot_height}" x2="{left + plot_width}" y2="{top + plot_height}" class="axis"/>')

    for tick in range(6):
        value = y_max * tick / 5
        y = y_pos(value)
        label = nice_number(value) if metric == "time_ns" else format_number(value)
        elements.append(f'<line x1="{left - 5}" y1="{y:.2f}" x2="{left + plot_width}" y2="{y:.2f}" class="grid"/>')
        elements.append(f'<text x="{left - 10}" y="{y + 4:.2f}" text-anchor="end" class="tick">{html.escape(label)}</text>')

    for size in sizes:
        x = x_pos(size)
        elements.append(f'<line x1="{x:.2f}" y1="{top + plot_height}" x2="{x:.2f}" y2="{top + plot_height + 5}" class="axis"/>')
        elements.append(f'<text x="{x:.2f}" y="{top + plot_height + 25}" text-anchor="middle" class="tick">{size}</text>')

    elements.append(f'<text x="{left + plot_width / 2}" y="{height - 12}" text-anchor="middle" class="label">Array size</text>')
    elements.append(f'<text x="18" y="{top + plot_height / 2}" text-anchor="middle" class="label" transform="rotate(-90 18 {top + plot_height / 2})">{html.escape(y_label)}</text>')

    legend_y = 48

    for index, (name, points) in enumerate(series.items()):
        color = palette[index % len(palette)]
        points = sorted(points)
        polyline = " ".join(f"{x_pos(size):.2f},{y_pos(value):.2f}" for size, value in points)

        elements.append(f'<polyline points="{polyline}" fill="none" stroke="{color}" stroke-width="2.5"/>')

        for size, value in points:
            x = x_pos(size)
            y = y_pos(value)
            label = nice_number(value) if metric == "time_ns" else format_number(value)
            elements.append(f'<circle cx="{x:.2f}" cy="{y:.2f}" r="4" fill="{color}"><title>{html.escape(name)} | size {size}: {html.escape(label)}</title></circle>')

        legend_x = width - 310
        y = legend_y + index * 20
        elements.append(f'<line x1="{legend_x}" y1="{y}" x2="{legend_x + 20}" y2="{y}" stroke="{color}" stroke-width="3"/>')
        elements.append(f'<text x="{legend_x + 28}" y="{y + 4}" class="legend">{html.escape(name)}</text>')

    elements.append("</svg>")

    return "\n".join(elements)

def make_table(rows):
    output = []
    output.append("<table>")
    output.append("<thead><tr><th>Size</th><th>Algorithm</th><th>Scenario</th><th>Time</th><th>Time, ns</th><th>Comparisons</th></tr></thead>")
    output.append("<tbody>")

    for row in rows:
        output.append(
            "<tr>"
            f"<td>{row['size']}</td>"
            f"<td>{html.escape(row['algorithm'])}</td>"
            f"<td>{html.escape(row['scenario'])}</td>"
            f"<td>{html.escape(row['time_text'])}</td>"
            f"<td>{format_number(row['time_ns'])}</td>"
            f"<td>{format_number(row['comparisons'])}</td>"
            "</tr>"
        )

    output.append("</tbody>")
    output.append("</table>")
    return "\n".join(output)

def make_html(sections):
    parts = []

    parts.append("""<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Search algorithms benchmark report</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
:root {
    color-scheme: light;
    font-family: Arial, Helvetica, sans-serif;
    background: #f3f4f6;
    color: #111827;
}
body {
    margin: 0;
    padding: 32px;
}
main {
    max-width: 1180px;
    margin: 0 auto;
}
h1 {
    margin: 0 0 24px;
    font-size: 30px;
}
h2 {
    margin: 0 0 16px;
    font-size: 22px;
}
section {
    background: white;
    border: 1px solid #e5e7eb;
    border-radius: 16px;
    padding: 24px;
    margin-bottom: 28px;
    box-shadow: 0 8px 24px rgba(15, 23, 42, 0.06);
}
.chart {
    width: 100%;
    height: auto;
    margin: 10px 0 24px;
    background: #ffffff;
    border: 1px solid #e5e7eb;
    border-radius: 12px;
}
.axis {
    stroke: #111827;
    stroke-width: 1;
}
.grid {
    stroke: #e5e7eb;
    stroke-width: 1;
}
.tick {
    font-size: 12px;
    fill: #4b5563;
}
.label {
    font-size: 13px;
    fill: #374151;
}
.legend {
    font-size: 12px;
    fill: #111827;
}
.chart-title {
    font-size: 17px;
    font-weight: 700;
    fill: #111827;
}
.table-wrap {
    overflow-x: auto;
}
table {
    width: 100%;
    border-collapse: collapse;
    font-size: 14px;
}
th, td {
    padding: 9px 12px;
    border-bottom: 1px solid #e5e7eb;
    text-align: left;
    white-space: nowrap;
}
th {
    background: #f9fafb;
    font-weight: 700;
}
tr:hover td {
    background: #f9fafb;
}
.summary {
    margin: 0 0 24px;
    color: #4b5563;
}
</style>
</head>
<body>
<main>
<h1>Search algorithms benchmark report</h1>
<p class="summary">Generated from searches_benchmark.txt. Time values are normalized to nanoseconds for charts.</p>
""")

    for section in sections:
        name = section["name"]
        rows = section["rows"]

        parts.append("<section>")
        parts.append(f"<h2>{html.escape(name)}</h2>")

        if rows:
            parts.append(make_svg_chart(rows, "time_ns", f"{name}: time", "Time"))
            parts.append(make_svg_chart(rows, "comparisons", f"{name}: comparisons", "Comparisons"))
            parts.append('<div class="table-wrap">')
            parts.append(make_table(rows))
            parts.append("</div>")
        else:
            parts.append("<p>No rows were parsed for this section.</p>")

        parts.append("</section>")

    parts.append("""
</main>
</body>
</html>
""")

    return "\n".join(parts)

def main():
    if not INPUT_FILE.exists():
        raise FileNotFoundError(f"Input file not found: {INPUT_FILE}")

    sections = parse_report(INPUT_FILE.read_text(encoding="utf-8"))
    OUTPUT_FILE.write_text(make_html(sections), encoding="utf-8")

    print(f"HTML report generated: {OUTPUT_FILE}")

if __name__ == "__main__":
    main()
