import tkinter as tk
import tkinter.font as tkfont
from tkinter import ttk, messagebox
import numpy as np
import math
import matplotlib

matplotlib.use("TkAgg")
matplotlib.rcParams.update({
    "font.size": 13,
    "axes.labelsize": 15,
    "xtick.labelsize": 11,
    "ytick.labelsize": 11,
})
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

from methods import newton_method, iter_method, F, get_reference_solution

def plot_system(x_range=(-2.5, 2.5), y_range=(-2.5, 2.5), n=400):
    xs = np.linspace(x_range[0], x_range[1], n)
    ys = np.linspace(y_range[0], y_range[1], n)
    X, Y = np.meshgrid(xs, ys)
    Z1 = np.sin(Y + 1.0) - X - 1.2
    Z2 = 2.0 * Y + np.cos(X) - 2.0
    return X, Y, Z1, Z2


def fmt(v):
    if v is None:
        return "—"
    v = float(v)
    if v == 0:
        return "0"
    if not np.isfinite(v):
        return str(v)
    return np.format_float_positional(v, trim='-').replace(".", ",")


def fmt_sci(v):
    return fmt(v)


def setup_fonts(root):
    for name, size, weight in (
        ("TkDefaultFont", 15, "normal"),
        ("TkTextFont", 15, "normal"),
        ("TkFixedFont", 15, "normal"),
        ("TkMenuFont", 15, "normal"),
        ("TkHeadingFont", 17, "bold"),
        ("TkCaptionFont", 17, "bold"),
        ("TkSmallCaptionFont", 15, "normal"),
        ("TkIconFont", 15, "normal"),
        ("TkTooltipFont", 15, "normal"),
    ):
        try:
            tkfont.nametofont(name).configure(size=size, weight=weight)
        except tk.TclError:
            pass
    style = ttk.Style(root)
    style.configure("Treeview", font=tkfont.nametofont("TkDefaultFont"), rowheight=34)
    style.configure("Treeview.Heading", font=tkfont.nametofont("TkHeadingFont"))


def build_header(master):
    header = tk.Frame(master)
    inner = tk.Frame(header)
    inner.pack(anchor="w", padx=8, pady=8)

    tk.Label(inner, text="Решение систем нелинейных уравнений",
             font=tkfont.nametofont("TkHeadingFont")).grid(
        row=0, column=0, columnspan=2, sticky="w")
    tk.Label(inner, text="Система:").grid(row=1, column=0, sticky="w", pady=(8, 0))
    tk.Label(inner, text="sin(y + 1) − x = 1,2").grid(row=2, column=0, sticky="w")
    tk.Label(inner, text="2y + cos(x) = 2").grid(row=3, column=0, sticky="w")
    tk.Label(inner, text="Приведённая форма:").grid(
        row=1, column=1, sticky="w", padx=(40, 0), pady=(8, 0))
    tk.Label(inner, text="sin(y + 1) − x − 1,2 = 0").grid(
        row=2, column=1, sticky="w", padx=(40, 0))
    tk.Label(inner, text="2y + cos(x) − 2 = 0").grid(
        row=3, column=1, sticky="w", padx=(40, 0))

    return header


class IterTable(tk.Frame):
    COLS = ("k", "x", "y", "dx", "dy", "d")
    HEAD = ("k", "x", "y", "Δx", "Δy", "δ")
    WIDTH = (40, 160, 160, 160, 160, 160)

    def __init__(self, master):
        super().__init__(master)
        self.tree = ttk.Treeview(self, columns=self.COLS, show="headings", height=5)
        for c, h, w in zip(self.COLS, self.HEAD, self.WIDTH):
            self.tree.heading(c, text=h, anchor="center")
            self.tree.column(c, width=w, minwidth=w, anchor="center", stretch=True)

        sb_y = ttk.Scrollbar(self, orient="vertical", command=self.tree.yview)
        sb_x = ttk.Scrollbar(self, orient="horizontal", command=self.tree.xview)
        self.tree.configure(yscrollcommand=sb_y.set, xscrollcommand=sb_x.set)

        self.tree.pack(side=tk.TOP, fill=tk.BOTH, expand=True)
        sb_y.pack(side=tk.RIGHT, fill=tk.Y)
        sb_x.pack(side=tk.BOTTOM, fill=tk.X)

    def clear(self):
        for i in self.tree.get_children():
            self.tree.delete(i)

    def fill(self, history):
        self.clear()
        for row in history:
            k, x, y, dx, dy, d, _, _ = row
            self.tree.insert("", "end",
                             values=(k, fmt(x), fmt(y), fmt_sci(dx), fmt_sci(dy), fmt_sci(d)))
        if self.tree.get_children():
            self.tree.see(self.tree.get_children()[-1])


class CompareTable(tk.Frame):
    COLS = ("method", "x", "y", "dx", "dy", "iters")
    HEAD = ("Метод", "x", "y", "|Δx|", "|Δy|", "Итераций")
    WIDTH = (200, 260, 260, 160, 160, 100)

    def __init__(self, master):
        super().__init__(master)
        self.tree = ttk.Treeview(self, columns=self.COLS, show="headings", height=4)
        for c, h, w in zip(self.COLS, self.HEAD, self.WIDTH):
            self.tree.heading(c, text=h, anchor="center")
            self.tree.column(c, width=w, minwidth=w, anchor="center", stretch=True)

        sb_x = ttk.Scrollbar(self, orient="horizontal", command=self.tree.xview)
        self.tree.configure(xscrollcommand=sb_x.set)

        self.tree.pack(side=tk.TOP, fill=tk.BOTH, expand=True)
        sb_x.pack(side=tk.BOTTOM, fill=tk.X)

    def fill(self, ref_x, ref_y, x_n, y_n, it_n, x_s, y_s, it_s, x_si, y_si, it_si):
        for i in self.tree.get_children():
            self.tree.delete(i)

        self.tree.insert("", "end", values=("Эталон", fmt(ref_x), fmt(ref_y), "—", "—", "—"))
        self.tree.insert("", "end",
                         values=("Ньютона", fmt(x_n), fmt(y_n), fmt_sci(abs(ref_x - x_n)),
                                 fmt_sci(abs(ref_y - y_n)), it_n))
        self.tree.insert("", "end",
                         values=("Зейделя", fmt(x_s), fmt(y_s), fmt_sci(abs(ref_x - x_s)),
                                 fmt_sci(abs(ref_y - y_s)), it_s))
        self.tree.insert("", "end",
                         values=("Простых итераций", fmt(x_si), fmt(y_si), fmt_sci(abs(ref_x - x_si)),
                                 fmt_sci(abs(ref_y - y_si)), it_si))


class ResultPanel(tk.Frame):
    def __init__(self, master):
        super().__init__(master)

        self.lbl_sol = tk.Label(self, text="—", justify="left", anchor="w")
        self.lbl_sol.pack(fill=tk.X)

        self.lbl_meta = tk.Label(self, text="", justify="left", anchor="w")
        self.lbl_meta.pack(fill=tk.X)

        self.lbl_check = tk.Label(self, text="", justify="left", anchor="w")
        self.lbl_check.pack(fill=tk.X)

        self.lbl_conv = tk.Label(self, text="", justify="left", anchor="w")
        self.lbl_conv.pack(fill=tk.X)

        self.lbl_warn = tk.Label(self, text="", justify="left", anchor="w")
        self.lbl_warn.pack(fill=tk.X)

    def set(self, x, y, iters, f1, f2, warn=None, conv_text=""):
        self.lbl_sol.config(text=f"x = {fmt(x)}\ny = {fmt(y)}")
        self.lbl_meta.config(text=f"Итераций: {iters}")
        s1 = "0" if abs(f1) < 1e-10 else fmt_sci(f1)
        s2 = "0" if abs(f2) < 1e-10 else fmt_sci(f2)
        self.lbl_check.config(text=f"f₁ = {s1}\nf₂ = {s2}")
        if conv_text:
            self.lbl_conv.config(text=conv_text)
            self.lbl_conv.pack(fill=tk.X)
        else:
            self.lbl_conv.pack_forget()
        self.lbl_warn.config(text=warn or "")


class GraphWindow(tk.Toplevel):
    def __init__(self, master, holder):
        super().__init__(master)
        self.title("График функций")
        self.geometry("800x650")
        self.holder = holder
        self._cur_xlim, self._cur_ylim = (-3.0, 3.0), (-3.0, 3.0)
        self.MIN_SPAN, self.MAX_SPAN, self.ZOOM_STEP = 0.02, 20.0, 1.4

        self.fig = Figure(figsize=(8, 6), dpi=100)
        self.ax = self.fig.add_subplot(111)

        self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        widget = self.canvas.get_tk_widget()
        widget.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        ctrl = tk.Frame(self)
        ctrl.pack(fill=tk.X, pady=(0, 10), padx=10)
        btn_box = tk.Frame(ctrl)
        btn_box.pack(side=tk.RIGHT)

        tk.Button(btn_box, text="−", command=self._zoom_out).pack(side=tk.LEFT, padx=(0, 8))
        tk.Button(btn_box, text="+", command=self._zoom_in).pack(side=tk.LEFT)
        self._draw_curves(self._cur_xlim, self._cur_ylim)

    def _validate_limits(self, xlim, ylim):
        xw, yh = xlim[1] - xlim[0], ylim[1] - ylim[0]
        if xw <= 0 or yh <= 0 or xw < self.MIN_SPAN or yh < self.MIN_SPAN or xw > self.MAX_SPAN or yh > self.MAX_SPAN:
            return False
        if not all(np.isfinite(v) for v in (*xlim, *ylim)):
            return False
        return True

    def _grid_n(self, xlim, ylim):
        span = max(xlim[1] - xlim[0], ylim[1] - ylim[0])
        lo, hi = self.MIN_SPAN, self.MAX_SPAN
        if span <= lo:
            n = 1200
        elif span >= hi:
            n = 400
        else:
            t = (np.log(span) - np.log(lo)) / (np.log(hi) - np.log(lo))
            n = int(1200 - t * 800)
        return max(300, min(1200, n))

    def _zoom_in(self):
        self._zoom_around_center(1 / self.ZOOM_STEP)

    def _zoom_out(self):
        self._zoom_around_center(self.ZOOM_STEP)

    def _zoom_around_center(self, factor):
        xl, yl = self.ax.get_xlim(), self.ax.get_ylim()
        cx, cy = self.holder.get("root", ((xl[0] + xl[1]) / 2, (yl[0] + yl[1]) / 2))
        xd, yd = (xl[1] - xl[0]) * factor / 2, (yl[1] - yl[0]) * factor / 2
        new_xlim, new_ylim = (cx - xd, cx + xd), (cy - yd, cy + yd)
        if self._validate_limits(new_xlim, new_ylim):
            self._cur_xlim, self._cur_ylim = new_xlim, new_ylim
            self._draw_curves(new_xlim, new_ylim)

    def _draw_curves(self, xlim, ylim):
        self.ax.clear()
        n = self._grid_n(xlim, ylim)
        X, Y, Z1, Z2 = plot_system(xlim, ylim, n=n)
        self.ax.contour(X, Y, np.nan_to_num(Z1, nan=1e9), levels=[0])
        self.ax.contour(X, Y, np.nan_to_num(Z2, nan=1e9), levels=[0])
        self.ax.axhline(0)
        self.ax.axvline(0)
        self.ax.grid(True)
        self.ax.ticklabel_format(style="plain", useOffset=False, axis="both")
        self.ax.set_xlabel("x")
        self.ax.set_ylabel("y")
        if "root" in self.holder:
            x, y = self.holder["root"]
            self.ax.plot(x, y, "o", ms=8)
        self.ax.set_xlim(*xlim)
        self.ax.set_ylim(*ylim)
        self.canvas.draw_idle()


class MethodTab(tk.Frame):
    def __init__(self, master, title, holder):
        super().__init__(master)
        self.holder = holder

        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)

        bottom = tk.Frame(self)
        bottom.grid(row=0, column=0, sticky="nsew", padx=8, pady=8)

        left_panel = ttk.LabelFrame(bottom, text=title)
        left_panel.pack(fill=tk.BOTH, expand=True)

        self.table = IterTable(left_panel)
        self.table.pack(fill=tk.BOTH, expand=True, padx=4, pady=4)

        self.result = ResultPanel(left_panel)
        self.result.pack(fill=tk.X, padx=4, pady=(0, 4))


class GaussTab(tk.Frame):
    def __init__(self, master):
        super().__init__(master)
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)

        outer = ttk.LabelFrame(self, text="Метод Гаусса")
        outer.grid(row=0, column=0, sticky="nsew", padx=8, pady=8)

        self.txt_gauss = tk.Text(outer, wrap="word")
        sb_y = ttk.Scrollbar(outer, orient="vertical", command=self.txt_gauss.yview)
        self.txt_gauss.configure(yscrollcommand=sb_y.set)

        self.txt_gauss.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=4, pady=4)
        sb_y.pack(side=tk.RIGHT, fill=tk.Y, padx=(0, 4), pady=4)


class SummaryTab(tk.Frame):
    def __init__(self, master, holder):
        super().__init__(master)
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)

        bottom = tk.Frame(self)
        bottom.grid(row=0, column=0, sticky="nsew", padx=8, pady=8)

        outer = ttk.LabelFrame(bottom, text="Сводная таблица")
        outer.pack(fill=tk.BOTH, expand=True)

        self.table = CompareTable(outer)
        self.table.pack(fill=tk.X, padx=4, pady=4)


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        setup_fonts(self)
        self.title("ЛР №3 — Решение СНУ")
        self.geometry("1900x1000")
        self.minsize(1750, 850)
        self.holder = {}

        self.rowconfigure(0, weight=0)
        self.rowconfigure(1, weight=1)
        self.columnconfigure(0, weight=1)

        top = tk.Frame(self)
        top.grid(row=0, column=0, sticky="ew", padx=8, pady=(8, 0))

        build_header(top).pack(side=tk.LEFT, anchor="n")

        ctrl_bar = tk.Frame(top)
        ctrl_bar.pack(side=tk.RIGHT, anchor="n", padx=(20, 0))

        tk.Label(ctrl_bar, text="ε:").pack(side=tk.LEFT, padx=(0, 6))
        self.entry_eps = tk.Entry(ctrl_bar, width=10, justify="center")
        self.entry_eps.pack(side=tk.LEFT, ipady=4, padx=(0, 12))
        self.entry_eps.insert(0, "0.00001")

        tk.Label(ctrl_bar, text="x₀ =").pack(side=tk.LEFT)
        self.entry_x0 = tk.Entry(ctrl_bar, width=8, justify="center")
        self.entry_x0.pack(side=tk.LEFT, ipady=4, padx=(4, 12))
        self.entry_x0.insert(0, "0.0")

        tk.Label(ctrl_bar, text="y₀ =").pack(side=tk.LEFT)
        self.entry_y0 = tk.Entry(ctrl_bar, width=8, justify="center")
        self.entry_y0.pack(side=tk.LEFT, ipady=4, padx=(4, 12))
        self.entry_y0.insert(0, "0.0")

        tk.Label(ctrl_bar, text="Итераций:").pack(side=tk.LEFT, padx=(0, 6))
        self.entry_max = tk.Entry(ctrl_bar, width=8, justify="center")
        self.entry_max.pack(side=tk.LEFT, ipady=4, padx=(0, 12))
        self.entry_max.insert(0, "10000")

        tk.Button(ctrl_bar, text="График", command=self._show_graph).pack(side=tk.LEFT, padx=(0, 6))
        tk.Button(ctrl_bar, text="Выполнить", command=self._run_all).pack(side=tk.LEFT)

        body = tk.Frame(self)
        body.grid(row=1, column=0, sticky="nsew", padx=8, pady=8)
        self.notebook = ttk.Notebook(body)
        self.notebook.pack(fill=tk.BOTH, expand=True)

        self.tab_newton = MethodTab(self.notebook, "Метод Ньютона", self.holder)
        self.notebook.add(self.tab_newton, text="Метод Ньютона")

        self.tab_gauss = GaussTab(self.notebook)
        self.notebook.add(self.tab_gauss, text="Метод Гаусса")

        self.tab_seidel = MethodTab(self.notebook, "Метод Зейделя", self.holder)
        self.notebook.add(self.tab_seidel, text="Метод Зейделя")

        self.tab_simple = MethodTab(self.notebook, "Метод простых итераций", self.holder)
        self.notebook.add(self.tab_simple, text="Метод простых итераций")

        self.tab_summary = SummaryTab(self.notebook, self.holder)
        self.notebook.add(self.tab_summary, text="Сводная таблица")

    def _read_params(self):
        try:
            eps = float(self.entry_eps.get().replace(",", "."))
            x0 = float(self.entry_x0.get().replace(",", "."))
            y0 = float(self.entry_y0.get().replace(",", "."))
            max_it = int(self.entry_max.get())
        except ValueError:
            raise ValueError("Проверьте ε, x₀, y₀ и max итераций.")
        if eps <= 0 or max_it <= 0:
            raise ValueError("ε и max итераций должны быть > 0.")
        return eps, x0, y0, max_it

    def _show_graph(self):
        GraphWindow(self, self.holder)

    def _run_all(self):
        try:
            eps, x0, y0, max_it = self._read_params()
        except ValueError as e:
            messagebox.showerror("Ошибка", str(e))
            return

        ref_x, ref_y = get_reference_solution()
        self.holder["root"] = (ref_x, ref_y)

        x_n, y_n, iters_n, history_n, gauss_logs, reason_n = newton_method(x0, y0, eps=eps, max_iter=max_it)
        self.tab_newton.table.fill(history_n)
        f1, f2 = F(x_n, y_n)
        warn_n = None if reason_n == "converged" else "Лимит итераций"
        self.tab_newton.result.set(x_n, y_n, iters_n, f1, f2, warn=warn_n)

        self.tab_gauss.txt_gauss.delete("1.0", tk.END)
        for k, log_lines in gauss_logs:
            self.tab_gauss.txt_gauss.insert(tk.END, f"=== Итерация {k} ===\n")
            for line in log_lines:
                self.tab_gauss.txt_gauss.insert(tk.END, line + "\n")
            self.tab_gauss.txt_gauss.insert(tk.END, "\n")

        x_s, y_s, iters_s, history_s, conv_s, reason_s, eq1_s, eq2_s = iter_method(
            x0, y0, eps=eps, max_iter=max_it, method_type="seidel")
        self.tab_seidel.table.fill(history_s)
        f1s, f2s = F(x_s, y_s)
        warn_s = None if reason_s == "converged" else reason_s
        conv_text_s = (
            f"Условия сходимости:\n"
            f"|∂φ₁/∂y| = {abs(math.cos(y0 + 1)):.4f} < 1 -> {'Да' if eq1_s else 'Нет'}\n"
            f"|∂φ₂/∂x| = {abs(math.sin(x0) / 2):.4f} < 1 -> {'Да' if eq2_s else 'Нет'}")
        self.tab_seidel.result.set(x_s, y_s, iters_s, f1s, f2s, warn=warn_s, conv_text=conv_text_s)

        x_si, y_si, iters_si, history_si, conv_si, reason_si, eq1_si, eq2_si = iter_method(
            x0, y0, eps=eps, max_iter=max_it, method_type="simple")
        self.tab_simple.table.fill(history_si)
        f1si, f2si = F(x_si, y_si)
        warn_si = None if reason_si == "converged" else reason_si
        conv_text_si = (
            f"Условия сходимости:\n"
            f"|∂φ₁/∂y| = {abs(math.cos(y0 + 1)):.4f} < 1 -> {'Да' if eq1_si else 'Нет'}\n"
            f"|∂φ₂/∂x| = {abs(math.sin(x0) / 2):.4f} < 1 -> {'Да' if eq2_si else 'Нет'}")
        self.tab_simple.result.set(x_si, y_si, iters_si, f1si, f2si, warn=warn_si, conv_text=conv_text_si)

        self.tab_summary.table.fill(ref_x, ref_y, x_n, y_n, iters_n, x_s, y_s, iters_s, x_si, y_si, iters_si)


if __name__ == "__main__":
    App().mainloop()
