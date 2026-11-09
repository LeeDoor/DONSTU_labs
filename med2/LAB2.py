import tkinter as tk
from tkinter import ttk, messagebox
import math

from solver_algorithms import (
    DECIMAL_PLACES,
    gauss_jordan,
    simple_iteration,
)

DEFAULT_TOLERANCE = 0.00001
FONT = ("Consolas", 14)
LABEL_FONT = ("Arial", 13)

class LinearSystemSolverApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Решение систем линейных уравнений")
        self.root.geometry("1450x950")
        self.root.minsize(1100, 750)

        self.n_var = tk.IntVar(value=3)
        self.tol_var = tk.StringVar(
            value=f"{DEFAULT_TOLERANCE:.{DECIMAL_PLACES}f}"
        )
        self.max_iter_var = tk.StringVar(value="100")

        self.matrix_entries = []
        self.rhs_entries = []

        self._build_ui()
        self._build_matrix_inputs()

    def _build_ui(self):
        outer = ttk.Frame(self.root, padding=12)
        outer.pack(fill="both", expand=True)

        controls = ttk.LabelFrame(
            outer,
            text="Параметры системы",
            padding=10
        )
        controls.pack(fill="x")

        ttk.Button(
            controls,
            text="Решить",
            command=self.solve
        ).grid(row=0, column=0, padx=(0, 20), sticky="w")

        ttk.Label(
            controls,
            text="Количество неизвестных:",
            font=LABEL_FONT
        ).grid(row=0, column=1, sticky="w")

        size_box = ttk.Combobox(
            controls,
            textvariable=self.n_var,
            values=(2, 3, 4, 5, 6),
            state="readonly",
            width=5,
            font=LABEL_FONT
        )
        size_box.grid(
            row=0, column=2, padx=(8, 20), sticky="w"
        )
        size_box.bind(
            "<<ComboboxSelected>>",
            lambda event: self._build_matrix_inputs()
        )

        ttk.Label(
            controls,
            text="Точность ε:",
            font=LABEL_FONT
        ).grid(row=0, column=3, sticky="w")

        ttk.Entry(
            controls,
            textvariable=self.tol_var,
            width=18,
            font=LABEL_FONT
        ).grid(row=0, column=4, padx=(8, 20))

        ttk.Label(
            controls,
            text="Максимум итераций:",
            font=LABEL_FONT
        ).grid(row=0, column=5, sticky="w")

        ttk.Entry(
            controls,
            textvariable=self.max_iter_var,
            width=9,
            font=LABEL_FONT
        ).grid(row=0, column=6, padx=8)

        self.matrix_frame = ttk.LabelFrame(
            outer,
            text="Коэффициенты системы Ax = b",
            padding=10
        )
        self.matrix_frame.pack(fill="x", pady=(12, 10))

        self.notebook = ttk.Notebook(outer)
        self.notebook.pack(fill="both", expand=True)

        self.gauss_tab = ttk.Frame(
            self.notebook,
            padding=10
        )
        self.iteration_tab = ttk.Frame(
            self.notebook,
            padding=10
        )

        self.notebook.add(
            self.gauss_tab,
            text="Метод Гаусса — Жордана"
        )
        self.notebook.add(
            self.iteration_tab,
            text="Метод простых итераций"
        )

        self.gauss_text = self._make_text(self.gauss_tab)
        self.iteration_text = self._make_text(
            self.iteration_tab
        )

    def _make_text(self, parent):
        frame = ttk.Frame(parent)
        frame.pack(fill="both", expand=True)

        text = tk.Text(
            frame,
            wrap="none",
            font=FONT,
            undo=False,
            padx=8,
            pady=8
        )

        ybar = ttk.Scrollbar(
            frame,
            orient="vertical",
            command=text.yview
        )

        xbar = ttk.Scrollbar(
            frame,
            orient="horizontal",
            command=text.xview
        )

        text.configure(
            yscrollcommand=ybar.set,
            xscrollcommand=xbar.set
        )

        text.grid(row=0, column=0, sticky="nsew")
        ybar.grid(row=0, column=1, sticky="ns")
        xbar.grid(row=1, column=0, sticky="ew")

        frame.rowconfigure(0, weight=1)
        frame.columnconfigure(0, weight=1)

        text.configure(state="disabled")
        return text

    def _build_matrix_inputs(self):
        for widget in self.matrix_frame.winfo_children():
            widget.destroy()

        n = self.n_var.get()

        self.matrix_entries = []
        self.rhs_entries = []

        for j in range(n):
            ttk.Label(
                self.matrix_frame,
                text=f"x{j + 1}",
                font=LABEL_FONT
            ).grid(row=0, column=j + 1, padx=8)

        ttk.Label(
            self.matrix_frame,
            text="b",
            font=LABEL_FONT
        ).grid(row=0, column=n + 1, padx=8)

        for i in range(n):
            row = []

            ttk.Label(
                self.matrix_frame,
                text=f"Уравнение {i + 1}:",
                font=LABEL_FONT
            ).grid(
                row=i + 1,
                column=0,
                sticky="w",
                padx=(0, 10)
            )

            for j in range(n):
                entry = ttk.Entry(
                    self.matrix_frame,
                    width=18,
                    justify="center",
                    font=LABEL_FONT
                )
                entry.grid(
                    row=i + 1,
                    column=j + 1,
                    padx=5,
                    pady=5
                )
                row.append(entry)

            rhs = ttk.Entry(
                self.matrix_frame,
                width=18,
                justify="center",
                font=LABEL_FONT
            )
            rhs.grid(
                row=i + 1,
                column=n + 1,
                padx=5,
                pady=5
            )

            self.matrix_entries.append(row)
            self.rhs_entries.append(rhs)

        self._fill_example()

    def _fill_example(self):
        a3 = [
            [56.43, -8.54, 6.36],
            [4.34, 49.87, 9.18],
            [6.75, -8.93, 48.88],
        ]

        b3 = [9.76, 43.48, 56.92]
        n = self.n_var.get()

        if n == 3:
            a, b = a3, b3
        else:
            a = [
                [
                    2.0 if i == j else 0.1
                    for j in range(n)
                ]
                for i in range(n)
            ]
            b = [sum(row) for row in a]

        for i in range(n):
            for j in range(n):
                self.matrix_entries[i][j].delete(0, "end")
                self.matrix_entries[i][j].insert(
                    0, str(a[i][j])
                )

            self.rhs_entries[i].delete(0, "end")
            self.rhs_entries[i].insert(0, str(b[i]))

    def _read_system(self):
        try:
            a = [
                [
                    float(entry.get().strip().replace(",", "."))
                    for entry in row
                ]
                for row in self.matrix_entries
            ]

            b = [
                float(entry.get().strip().replace(",", "."))
                for entry in self.rhs_entries
            ]

            tol = float(
                self.tol_var.get().strip().replace(",", ".")
            )
            max_iter = int(self.max_iter_var.get())

            if not math.isfinite(tol) or tol <= 0:
                raise ValueError(
                    "Точность должна быть положительным конечным числом."
                )

            if max_iter <= 0:
                raise ValueError(
                    "Максимум итераций должен быть положительным целым числом."
                )

            if not all(
                math.isfinite(v)
                for row in a
                for v in row
            ):
                raise ValueError(
                    "Коэффициенты матрицы должны быть конечными числами."
                )

            if not all(math.isfinite(v) for v in b):
                raise ValueError(
                    "Правая часть должна содержать конечные числа."
                )

            return a, b, tol, max_iter

        except ValueError as exc:
            raise ValueError(
                f"Проверьте входные данные: {exc}"
            ) from exc

    @staticmethod
    def _show(widget, content):
        widget.configure(state="normal")
        widget.delete("1.0", "end")
        widget.insert("1.0", content)
        widget.configure(state="disabled")

    def solve(self):
        try:
            a, b, tol, max_iter = self._read_system()

            if self.notebook.index("current") == 0:
                _, log = gauss_jordan(a, b, tol)
                self._show(self.gauss_text, log)
            else:
                _, log = simple_iteration(
                    a, b, tol, max_iter
                )
                self._show(self.iteration_text, log)

        except Exception as exc:
            messagebox.showerror("Ошибка решения", str(exc))


def main():
    root = tk.Tk()
    LinearSystemSolverApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
