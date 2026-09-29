import math
import tkinter as tk
from tkinter import ttk, messagebox, filedialog
from dataclasses import dataclass
from typing import Callable

import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure


EPSILON = 1e-5
DEFAULT_LEFT = -10.0
DEFAULT_RIGHT = 10.0
DEFAULT_STEP = 0.01
MAX_ITERATIONS = 1000


@dataclass
class RootInterval:
    left: float
    right: float


@dataclass
class Iteration:
    number: int
    x: float
    fx: float
    error: float


@dataclass
class MethodResult:
    root: float
    residual: float
    iterations: int
    history: list[Iteration]


@dataclass
class Equation:
    name: str
    expression: str
    function: Callable[[float], float]


def equation_1(x: float) -> float:
    return 2.0 * math.sin(x - 0.6) - 1.5 + x


def equation_2(x: float) -> float:
    return (x - 2.0) ** 2 - math.exp(x) / 2.0


EQUATIONS = [
    Equation(
        "Уравнение 1",
        "2 sin(x - 0.6) = 1.5 - x",
        equation_1,
    ),
    Equation(
        "Уравнение 2",
        "(x - 2)^2 = exp(x) / 2",
        equation_2,
    ),
]


def localize_roots(
    function: Callable[[float], float],
    left: float,
    right: float,
    step: float,
) -> list[RootInterval]:
    if left >= right:
        raise ValueError("The left boundary must be smaller than the right boundary.")
    if step <= 0:
        raise ValueError("The localization step must be positive.")

    intervals = []
    x1 = left
    f1 = function(x1)

    if math.isclose(f1, 0.0, abs_tol=1e-14):
        intervals.append(RootInterval(x1 - step / 2.0, x1 + step / 2.0))

    while x1 < right:
        x2 = min(x1 + step, right)
        f2 = function(x2)

        if not math.isfinite(f1) or not math.isfinite(f2):
            x1, f1 = x2, f2
            continue

        if f1 * f2 < 0.0:
            intervals.append(RootInterval(x1, x2))
        elif math.isclose(f2, 0.0, abs_tol=1e-14):
            intervals.append(RootInterval(x2 - step / 2.0, x2 + step / 2.0))

        x1, f1 = x2, f2

    unique = []
    for interval in intervals:
        if not unique:
            unique.append(interval)
            continue

        previous = unique[-1]
        if abs(interval.left - previous.right) > step * 0.51:
            unique.append(interval)
        else:
            previous.right = max(previous.right, interval.right)

    return unique


def bisection(
    function: Callable[[float], float],
    left: float,
    right: float,
    epsilon: float,
) -> MethodResult:
    f_left = function(left)
    f_right = function(right)

    if f_left == 0.0:
        return MethodResult(left, 0.0, 0, [])
    if f_right == 0.0:
        return MethodResult(right, 0.0, 0, [])
    if f_left * f_right > 0.0:
        raise ValueError("Bisection requires an interval with opposite endpoint signs.")

    history = []
    previous = None

    for iteration in range(1, MAX_ITERATIONS + 1):
        middle = (left + right) / 2.0
        f_middle = function(middle)
        error = abs(middle - previous) if previous is not None else abs(right - left) / 2.0

        history.append(Iteration(iteration, middle, f_middle, error))

        if abs(f_middle) <= epsilon or abs(right - left) / 2.0 <= epsilon:
            return MethodResult(
                middle,
                abs(f_middle),
                iteration,
                history,
            )

        if f_left * f_middle < 0.0:
            right = middle
            f_right = f_middle
        else:
            left = middle
            f_left = f_middle

        previous = middle

    raise RuntimeError("Bisection did not converge within the iteration limit.")


def secant(
    function: Callable[[float], float],
    x0: float,
    x1: float,
    epsilon: float,
) -> MethodResult:
    f0 = function(x0)
    f1 = function(x1)

    history = [
        Iteration(0, x0, f0, float("nan")),
        Iteration(1, x1, f1, abs(x1 - x0)),
    ]

    for iteration in range(2, MAX_ITERATIONS + 1):
        denominator = f1 - f0

        if denominator == 0.0:
            raise ZeroDivisionError("The secant denominator became zero.")

        x2 = x1 - f1 * (x1 - x0) / denominator
        f2 = function(x2)
        error = abs(x2 - x1)

        history.append(Iteration(iteration, x2, f2, error))

        if error <= epsilon or abs(f2) <= epsilon:
            return MethodResult(
                x2,
                abs(f2),
                iteration,
                history,
            )

        x0, f0 = x1, f1
        x1, f1 = x2, f2

    raise RuntimeError("The secant method did not converge within the iteration limit.")


def format_number(value: float) -> str:
    return f"{value:.10f}"


class RootFinderApp:
    def __init__(self, root: tk.Tk):
        self.root = root
        self.root.title("Численные методы решения нелинейных уравнений")
        self.root.geometry("1280x820")
        self.root.minsize(1050, 700)

        self.current_equation = EQUATIONS[0]
        self.results = []
        self.intervals = []

        self.equation_var = tk.StringVar(value=EQUATIONS[0].name)
        self.left_var = tk.StringVar(value=str(DEFAULT_LEFT))
        self.right_var = tk.StringVar(value=str(DEFAULT_RIGHT))
        self.step_var = tk.StringVar(value=str(DEFAULT_STEP))
        self.epsilon_var = tk.StringVar(value=str(EPSILON))
        self.status_var = tk.StringVar(value="Готово.")

        self._build_ui()
        self._update_equation_text()
        self.calculate()

    def _build_ui(self):
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(1, weight=1)

        controls = ttk.Frame(self.root, padding=12)
        controls.grid(row=0, column=0, sticky="ew")
        controls.columnconfigure(5, weight=1)

        ttk.Label(controls, text="Уравнение:").grid(row=0, column=0, padx=(0, 6), pady=4)
        equation_box = ttk.Combobox(
            controls,
            textvariable=self.equation_var,
            values=[equation.name for equation in EQUATIONS],
            state="readonly",
            width=18,
        )
        equation_box.grid(row=0, column=1, padx=6, pady=4)
        equation_box.bind("<<ComboboxSelected>>", self._equation_changed)

        self.expression_label = ttk.Label(controls, text="")
        self.expression_label.grid(row=0, column=2, padx=12, pady=4, sticky="w")

        ttk.Label(controls, text="Левая граница:").grid(row=1, column=0, padx=(0, 6), pady=4)
        ttk.Entry(controls, textvariable=self.left_var, width=12).grid(row=1, column=1, padx=6, pady=4)

        ttk.Label(controls, text="Правая граница:").grid(row=1, column=2, padx=(12, 6), pady=4)
        ttk.Entry(controls, textvariable=self.right_var, width=12).grid(row=1, column=3, padx=6, pady=4)

        ttk.Label(controls, text="Шаг локализации:").grid(row=1, column=4, padx=(12, 6), pady=4)
        ttk.Entry(controls, textvariable=self.step_var, width=12).grid(row=1, column=5, padx=6, pady=4, sticky="w")

        ttk.Label(controls, text="ε:").grid(row=1, column=6, padx=(12, 6), pady=4)
        ttk.Entry(controls, textvariable=self.epsilon_var, width=12).grid(row=1, column=7, padx=6, pady=4)

        ttk.Button(controls, text="Рассчитать", command=self.calculate).grid(
            row=0, column=7, rowspan=2, padx=(18, 0), pady=4
        )

        ttk.Button(controls, text="Сохранить отчет", command=self.save_report).grid(
            row=0, column=8, rowspan=2, padx=(8, 0), pady=4
        )

        notebook = ttk.Notebook(self.root)
        notebook.grid(row=1, column=0, sticky="nsew", padx=12, pady=(0, 8))

        graph_tab = ttk.Frame(notebook)
        result_tab = ttk.Frame(notebook)
        iteration_tab = ttk.Frame(notebook)

        notebook.add(graph_tab, text="График")
        notebook.add(result_tab, text="Результаты")
        notebook.add(iteration_tab, text="Итерации")

        graph_tab.rowconfigure(0, weight=1)
        graph_tab.columnconfigure(0, weight=1)

        self.figure = Figure(figsize=(9, 6), dpi=100)
        self.axes = self.figure.add_subplot(111)
        self.canvas = FigureCanvasTkAgg(self.figure, master=graph_tab)
        self.canvas.get_tk_widget().grid(row=0, column=0, sticky="nsew")

        result_tab.rowconfigure(0, weight=1)
        result_tab.columnconfigure(0, weight=1)

        result_frame = ttk.Frame(result_tab, padding=10)
        result_frame.grid(row=0, column=0, sticky="nsew")
        result_frame.rowconfigure(1, weight=1)
        result_frame.columnconfigure(0, weight=1)

        self.localization_label = ttk.Label(result_frame, text="", font=("TkDefaultFont", 10, "bold"))
        self.localization_label.grid(row=0, column=0, sticky="w", pady=(0, 10))

        result_columns = (
            "root_number",
            "interval",
            "bisection_root",
            "bisection_iterations",
            "bisection_residual",
            "secant_root",
            "secant_iterations",
            "secant_residual",
        )

        self.result_tree = ttk.Treeview(result_frame, columns=result_columns, show="headings")
        headings = {
            "root_number": "Корень",
            "interval": "Интервал",
            "bisection_root": "Бисекция: x",
            "bisection_iterations": "Бисекция: итерации",
            "bisection_residual": "Бисекция: |f(x)|",
            "secant_root": "Секущая: x",
            "secant_iterations": "Секущая: итерации",
            "secant_residual": "Секущая: |f(x)|",
        }

        widths = {
            "root_number": 70,
            "interval": 150,
            "bisection_root": 145,
            "bisection_iterations": 125,
            "bisection_residual": 145,
            "secant_root": 145,
            "secant_iterations": 125,
            "secant_residual": 145,
        }

        for column in result_columns:
            self.result_tree.heading(column, text=headings[column])
            self.result_tree.column(column, width=widths[column], anchor="center")

        self.result_tree.grid(row=1, column=0, sticky="nsew")

        result_scroll = ttk.Scrollbar(result_frame, orient="vertical", command=self.result_tree.yview)
        result_scroll.grid(row=1, column=1, sticky="ns")
        self.result_tree.configure(yscrollcommand=result_scroll.set)

        iteration_tab.rowconfigure(0, weight=1)
        iteration_tab.columnconfigure(0, weight=1)

        iteration_frame = ttk.Frame(iteration_tab, padding=10)
        iteration_frame.grid(row=0, column=0, sticky="nsew")
        iteration_frame.rowconfigure(1, weight=1)
        iteration_frame.columnconfigure(0, weight=1)

        top_iteration_frame = ttk.Frame(iteration_frame)
        top_iteration_frame.grid(row=0, column=0, sticky="ew", pady=(0, 8))

        ttk.Label(top_iteration_frame, text="Корень:").pack(side="left")
        self.root_selector = ttk.Combobox(
            top_iteration_frame,
            state="readonly",
            width=8,
        )
        self.root_selector.pack(side="left", padx=(5, 15))
        self.root_selector.bind("<<ComboboxSelected>>", self._root_iteration_changed)

        ttk.Label(top_iteration_frame, text="Метод:").pack(side="left")
        self.method_selector = ttk.Combobox(
            top_iteration_frame,
            state="readonly",
            values=["Метод половинного деления", "Метод секущих"],
            width=26,
        )
        self.method_selector.current(0)
        self.method_selector.pack(side="left", padx=5)
        self.method_selector.bind("<<ComboboxSelected>>", self._method_iteration_changed)

        iteration_columns = ("iteration", "x", "fx", "error")
        self.iteration_tree = ttk.Treeview(
            iteration_frame,
            columns=iteration_columns,
            show="headings",
        )

        iteration_headings = {
            "iteration": "Итерация",
            "x": "x",
            "fx": "f(x)",
            "error": "Оценка погрешности",
        }

        for column in iteration_columns:
            self.iteration_tree.heading(column, text=iteration_headings[column])
            self.iteration_tree.column(column, anchor="center", width=220)

        self.iteration_tree.grid(row=1, column=0, sticky="nsew")

        iteration_scroll = ttk.Scrollbar(
            iteration_frame,
            orient="vertical",
            command=self.iteration_tree.yview,
        )
        iteration_scroll.grid(row=1, column=1, sticky="ns")
        self.iteration_tree.configure(yscrollcommand=iteration_scroll.set)

        status = ttk.Label(
            self.root,
            textvariable=self.status_var,
            relief="sunken",
            anchor="w",
            padding=(8, 4),
        )
        status.grid(row=2, column=0, sticky="ew")

    def _equation_changed(self, _event=None):
        selected = self.equation_var.get()
        self.current_equation = next(equation for equation in EQUATIONS if equation.name == selected)
        self._update_equation_text()
        self.calculate()

    def _update_equation_text(self):
        self.expression_label.configure(
            text=f"f(x) = {self.current_equation.expression}"
        )

    def _parse_input(self):
        try:
            left = float(self.left_var.get().replace(",", "."))
            right = float(self.right_var.get().replace(",", "."))
            step = float(self.step_var.get().replace(",", "."))
            epsilon = float(self.epsilon_var.get().replace(",", "."))

            if left >= right:
                raise ValueError("The left boundary must be smaller than the right boundary.")
            if step <= 0:
                raise ValueError("The localization step must be positive.")
            if epsilon <= 0:
                raise ValueError("Epsilon must be positive.")

            return left, right, step, epsilon
        except ValueError as error:
            raise ValueError(str(error))

    def calculate(self):
        try:
            left, right, step, epsilon = self._parse_input()

            self.intervals = localize_roots(
                self.current_equation.function,
                left,
                right,
                step,
            )

            if not self.intervals:
                raise ValueError(
                    "No roots were localized. Increase the search interval or decrease the localization step."
                )

            self.results = []

            for interval in self.intervals:
                bisection_result = bisection(
                    self.current_equation.function,
                    interval.left,
                    interval.right,
                    epsilon,
                )

                secant_result = secant(
                    self.current_equation.function,
                    interval.left,
                    interval.right,
                    epsilon,
                )

                self.results.append((bisection_result, secant_result))

            self._update_result_table()
            self._update_iteration_selectors()
            self._update_iteration_table()
            self._draw_graph(left, right)
            self.status_var.set(
                f"Расчет завершен. Локализовано корней: {len(self.intervals)}. ε = {epsilon:g}"
            )

        except Exception as error:
            self.status_var.set("Ошибка расчета.")
            messagebox.showerror("Ошибка", str(error))

    def _update_result_table(self):
        for item in self.result_tree.get_children():
            self.result_tree.delete(item)

        interval_texts = []

        for index, interval in enumerate(self.intervals, start=1):
            bisection_result, secant_result = self.results[index - 1]

            interval_text = f"[{format_number(interval.left)}, {format_number(interval.right)}]"
            interval_texts.append(f"{index}: {interval_text}")

            self.result_tree.insert(
                "",
                "end",
                values=(
                    index,
                    interval_text,
                    format_number(bisection_result.root),
                    bisection_result.iterations,
                    f"{bisection_result.residual:.3e}",
                    format_number(secant_result.root),
                    secant_result.iterations,
                    f"{secant_result.residual:.3e}",
                ),
            )

        self.localization_label.configure(
            text="Локализованные корни: " + "; ".join(interval_texts)
        )

    def _update_iteration_selectors(self):
        values = [str(index) for index in range(1, len(self.results) + 1)]
        self.root_selector.configure(values=values)

        if values:
            self.root_selector.current(0)

    def _root_iteration_changed(self, _event=None):
        self._update_iteration_table()

    def _method_iteration_changed(self, _event=None):
        self._update_iteration_table()

    def _update_iteration_table(self):
        for item in self.iteration_tree.get_children():
            self.iteration_tree.delete(item)

        if not self.results:
            return

        root_index = self.root_selector.current()
        if root_index < 0:
            root_index = 0

        method_index = self.method_selector.current()

        result = self.results[root_index][method_index]

        for item in result.history:
            error_text = "—" if not math.isfinite(item.error) else f"{item.error:.10e}"

            self.iteration_tree.insert(
                "",
                "end",
                values=(
                    item.number,
                    f"{item.x:.12f}",
                    f"{item.fx:.6e}",
                    error_text,
                ),
            )

    def _draw_graph(self, left: float, right: float):
        self.axes.clear()

        padding = max((right - left) * 0.04, 0.1)
        graph_left = left - padding
        graph_right = right + padding

        xs = np.linspace(graph_left, graph_right, 2000)
        ys = []

        for x in xs:
            try:
                value = self.current_equation.function(float(x))
                ys.append(value if math.isfinite(value) else np.nan)
            except (ValueError, OverflowError):
                ys.append(np.nan)

        ys = np.asarray(ys)

        finite_values = ys[np.isfinite(ys)]

        if finite_values.size:
            y_limit = np.nanpercentile(np.abs(finite_values), 97)
            if not math.isfinite(y_limit) or y_limit <= 0:
                y_limit = 1.0
            y_limit = min(y_limit * 1.3, max(np.nanmax(np.abs(finite_values)) * 1.1, 1.0))
        else:
            y_limit = 1.0

        self.axes.plot(xs, ys, label="f(x)")
        self.axes.axhline(0.0, linewidth=1.0)
        self.axes.axvline(0.0, linewidth=1.0)

        for index, interval in enumerate(self.intervals, start=1):
            self.axes.axvspan(
                interval.left,
                interval.right,
                alpha=0.18,
                label=f"Локализация {index}" if index == 1 else None,
            )

            bisection_result, secant_result = self.results[index - 1]

            self.axes.scatter(
                [bisection_result.root],
                [self.current_equation.function(bisection_result.root)],
                marker="o",
                s=45,
                label="Бисекция" if index == 1 else None,
                zorder=5,
            )

            self.axes.scatter(
                [secant_result.root],
                [self.current_equation.function(secant_result.root)],
                marker="x",
                s=60,
                label="Секущая" if index == 1 else None,
                zorder=5,
            )

        self.axes.set_ylim(-y_limit, y_limit)
        self.axes.set_xlim(graph_left, graph_right)
        self.axes.set_xlabel("x")
        self.axes.set_ylabel("f(x)")
        self.axes.set_title(
            f"{self.current_equation.name}: {self.current_equation.expression}"
        )
        self.axes.grid(True, alpha=0.25)
        self.axes.legend()
        self.figure.tight_layout()
        self.canvas.draw()

    def _build_report(self):
        lines = [
            "NUMERICAL METHODS REPORT",
            "",
            f"Equation: {self.current_equation.name}",
            f"Function: f(x) = {self.current_equation.expression}",
            f"Search interval: [{self.left_var.get()}, {self.right_var.get()}]",
            f"Localization step: {self.step_var.get()}",
            f"Epsilon: {self.epsilon_var.get()}",
            "",
            "LOCALIZED ROOT INTERVALS",
        ]

        for index, interval in enumerate(self.intervals, start=1):
            lines.append(
                f"{index}. [{format_number(interval.left)}, {format_number(interval.right)}]"
            )

        lines.extend(
            [
                "",
                "RESULT COMPARISON",
                "Root | Bisection root | Bisection iterations | Bisection residual | "
                "Secant root | Secant iterations | Secant residual",
            ]
        )

        for index, (bisection_result, secant_result) in enumerate(self.results, start=1):
            lines.append(
                f"{index} | "
                f"{format_number(bisection_result.root)} | "
                f"{bisection_result.iterations} | "
                f"{bisection_result.residual:.6e} | "
                f"{format_number(secant_result.root)} | "
                f"{secant_result.iterations} | "
                f"{secant_result.residual:.6e}"
            )

        lines.extend(
            [
                "",
                "ACCURACY CHECK",
                "The residual |f(x)| is evaluated directly after each method. "
                "The difference between the roots obtained by the two methods is also reported.",
            ]
        )

        for index, (bisection_result, secant_result) in enumerate(self.results, start=1):
            difference = abs(bisection_result.root - secant_result.root)
            lines.append(
                f"Root {index}: |x_bisection - x_secant| = {difference:.6e}"
            )

        lines.extend(["", "ITERATION DETAILS"])

        for index, (bisection_result, secant_result) in enumerate(self.results, start=1):
            lines.append("")
            lines.append(f"Root {index}: BISECTION")
            lines.append("iteration | x | f(x) | error")

            for item in bisection_result.history:
                error_text = "nan" if not math.isfinite(item.error) else f"{item.error:.10e}"
                lines.append(
                    f"{item.number} | {item.x:.12f} | {item.fx:.6e} | {error_text}"
                )

            lines.append("")
            lines.append(f"Root {index}: SECANT")
            lines.append("iteration | x | f(x) | error")

            for item in secant_result.history:
                error_text = "nan" if not math.isfinite(item.error) else f"{item.error:.10e}"
                lines.append(
                    f"{item.number} | {item.x:.12f} | {item.fx:.6e} | {error_text}"
                )

        return "\n".join(lines)

    def save_report(self):
        if not self.results:
            messagebox.showwarning("Отчет", "Сначала выполните расчет.")
            return

        filename = filedialog.asksaveasfilename(
            title="Сохранить отчет",
            defaultextension=".txt",
            filetypes=[
                ("Text files", "*.txt"),
                ("All files", "*.*"),
            ],
        )

        if not filename:
            return

        try:
            with open(filename, "w", encoding="utf-8") as file:
                file.write(self._build_report())

            messagebox.showinfo("Отчет", f"Отчет сохранен:\n{filename}")
        except OSError as error:
            messagebox.showerror("Ошибка", str(error))


def main():
    root = tk.Tk()
    RootFinderApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()

