import math
import re
import html
import sympy as sp

from PyQt5.QtWidgets import (
    QWidget, QLabel, QPushButton, QTextEdit, QVBoxLayout, QHBoxLayout,
    QGroupBox, QMessageBox, QSizePolicy, QComboBox, QLineEdit, QSpinBox,
    QFormLayout
)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFont

from algorithms import (
    gauss_krauth,
    simple_iterations,
    simple_iterations_transposed,
    simple_iterations_raw
)


class MainWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Численные методы решения СЛАУ")
        self.resize(1450, 900)
        self.setMinimumSize(950, 650)

        self.setStyleSheet("""
            QWidget {
                background-color: #fff0f6;
                color: #512b3b;
                font-size: 30px;
            }

            QGroupBox {
                background-color: #fff8fb;
                border: 1px solid #e6b1c7;
                border-radius: 12px;
                margin-top: 14px;
                padding: 12px;
                font-weight: bold;
            }

            QGroupBox::title {
                subcontrol-origin: margin;
                left: 14px;
                padding: 0 6px;
            }

            QPushButton {
                background-color: #df83a9;
                color: white;
                border: none;
                border-radius: 9px;
                padding: 9px;
                font-weight: bold;
            }

            QPushButton:hover {
                background-color: #cb6892;
            }

            QTextEdit, QLineEdit, QComboBox, QSpinBox {
                background-color: white;
                border: 1px solid #ead0dc;
                border-radius: 7px;
                padding: 6px;
            }
        """)

        self.create_interface()

    def create_interface(self):
        main_layout = QVBoxLayout()
        main_layout.setContentsMargins(18, 14, 18, 14)
        main_layout.setSpacing(10)

        title = QLabel("Решение систем линейных уравнений")
        title.setAlignment(Qt.AlignCenter)
        title.setFont(QFont("Arial", 32, QFont.Bold))
        main_layout.addWidget(title)

        # Панель ввода.
        input_group = QGroupBox("Система уравнений")
        input_layout = QVBoxLayout()

        hint = QLabel(
            "Введите каждое уравнение с новой строки. "
            "Число уравнений должно совпадать с числом неизвестных.\n"
            "Пример: 2,34*x1 - 4,21*x2 = 14,41"
        )
        hint.setWordWrap(True)
        input_layout.addWidget(hint)

        self.equations_input = QTextEdit()
        self.equations_input.setMinimumHeight(180)
        self.equations_input.setSizePolicy(
            QSizePolicy.Expanding,
            QSizePolicy.Expanding
        )

        self.equations_input.setPlainText(
            "x1 + 5*x2 - x3 = 2\n"
            "x1 + 2*x3 = -1\n"
            "2*x1 - x2 - 3*x3 = 5"
        )

        input_layout.addWidget(self.equations_input, 1)

        settings = QFormLayout()

        self.method_choice = QComboBox()
        self.method_choice.addItems([
            "Метод Гаусса — Краута",
            "Метод простых итераций"
        ])
        settings.addRow("Метод:", self.method_choice)

        self.epsilon_input = QLineEdit("0,000001")
        settings.addRow("Точность ε:", self.epsilon_input)

        self.max_iterations_input = QSpinBox()
        self.max_iterations_input.setRange(1, 1000000)
        self.max_iterations_input.setValue(1000)
        settings.addRow("Максимум итераций k_max:", self.max_iterations_input)

        input_layout.addLayout(settings)

        buttons = QHBoxLayout()

        solve_button = QPushButton("Решить систему")
        solve_button.clicked.connect(self.solve)
        buttons.addWidget(solve_button)

        clear_button = QPushButton("Очистить результат")
        clear_button.clicked.connect(self.clear_result)
        buttons.addWidget(clear_button)

        input_layout.addLayout(buttons)
        input_group.setLayout(input_layout)

        # Единая область результатов для обоих методов.
        result_group = QGroupBox("Ход решения")
        result_layout = QVBoxLayout()

        self.result_output = QTextEdit()
        self.result_output.setReadOnly(True)
        self.result_output.setLineWrapMode(QTextEdit.NoWrap)
        self.result_output.setSizePolicy(
            QSizePolicy.Expanding,
            QSizePolicy.Expanding
        )

        result_layout.addWidget(self.result_output)
        result_group.setLayout(result_layout)

        panels = QHBoxLayout()
        panels.setSpacing(14)
        panels.addWidget(input_group, 2)
        panels.addWidget(result_group, 5)

        main_layout.addLayout(panels, 1)
        self.setLayout(main_layout)

    def parse_equations(self):
        """Преобразует введённые уравнения в матрицу A и вектор B."""
        text = self.equations_input.toPlainText().strip()

        if not text:
            raise ValueError("Введите систему уравнений.")

        lines = [
            line.strip()
            for line in text.splitlines()
            if line.strip()
        ]

        n = len(lines)

        variables = tuple(
            sp.Symbol(f"x{i + 1}")
            for i in range(n)
        )

        local_vars = {
            f"x{i + 1}": variables[i]
            for i in range(n)
        }

        allowed = re.compile(r"^[0-9xX+\-*/().,\s]+$")

        A = []
        B = []

        for i, line in enumerate(lines):
            if line.count("=") != 1:
                raise ValueError(
                    f"Уравнение {i + 1}: "
                    "должен быть ровно один знак '='."
                )

            left, right = line.split("=")

            left = left.replace(",", ".").replace("X", "x")
            right = right.replace(",", ".").replace("X", "x")

            if (
                not allowed.fullmatch(left)
                or not allowed.fullmatch(right)
            ):
                raise ValueError(
                    f"Уравнение {i + 1} содержит "
                    "недопустимые символы."
                )

            try:
                expr = sp.expand(
                    sp.sympify(left, locals=local_vars)
                    - sp.sympify(right, locals=local_vars)
                )

                polynomial = sp.Poly(expr, *variables)

            except (
                sp.SympifyError,
                SyntaxError,
                TypeError,
                sp.PolynomialError
            ) as error:
                raise ValueError(
                    f"Уравнение {i + 1} "
                    "не удалось разобрать как линейное."
                ) from error

            if polynomial.total_degree() > 1:
                raise ValueError(
                    f"Уравнение {i + 1} не является линейным."
                )

            row = []

            for variable in variables:
                coefficient = polynomial.coeff_monomial(variable)

                if coefficient.free_symbols:
                    raise ValueError(
                        f"Уравнение {i + 1} содержит "
                        "недопустимый коэффициент."
                    )

                row.append(float(coefficient))

            constant = polynomial.coeff_monomial(1)

            if constant.free_symbols:
                raise ValueError(
                    f"Уравнение {i + 1} содержит "
                    "недопустимую константу."
                )

            A.append(row)
            B.append(float(-constant))

        if (
            not all(
                math.isfinite(v)
                for row in A
                for v in row
            )
            or not all(math.isfinite(v) for v in B)
        ):
            raise ValueError(
                "Коэффициенты должны быть конечными числами."
            )

        return A, B

    def print_matrix(self, title, A, B):
        """Выводит расширенную матрицу [A | B]."""
        n = len(B)

        if n == 1:
            left_brackets, right_brackets = ["("], [")"]
        elif n == 2:
            left_brackets = ["⎛", "⎝"]
            right_brackets = ["⎞", "⎠"]
        else:
            left_brackets = ["⎛"] + ["⎜"] * (n - 2) + ["⎝"]
            right_brackets = ["⎞"] + ["⎟"] * (n - 2) + ["⎠"]

        parts = [
            '<div style="margin:8px 0 18px 0;">',
            '<div style="font-size:32px;font-weight:bold;'
            'color:#713650;margin-bottom:8px;">',
            html.escape(title),
            '</div>',
            '<table style="border-collapse:collapse;'
            'font-family:Consolas,monospace;font-size:30px;">'
        ]

        for i in range(n):
            parts.append("<tr>")

            parts.append(
                '<td style="padding-right:7px;'
                'font-family:serif;font-size:44px;color:#c05b88;">'
                f'{left_brackets[i]}</td>'
            )

            for j, value in enumerate(A[i]):
                border = (
                    "border-right:3px solid #c05b88;"
                    if j == len(A[i]) - 1
                    else ""
                )

                parts.append(
                    '<td style="padding:5px 9px;text-align:right;'
                    f'white-space:nowrap;{border}">'
                    f'{value:.10f}</td>'
                )

            parts.append(
                '<td style="padding:5px 9px 5px 13px;'
                'text-align:right;white-space:nowrap;'
                'font-weight:bold;color:#713650;">'
                f'{B[i]:.10f}</td>'
            )

            parts.append(
                '<td style="padding-left:7px;font-family:serif;'
                'font-size:44px;color:#c05b88;">'
                f'{right_brackets[i]}</td>'
            )

            parts.append("</tr>")

        parts.append("</table></div>")
        self.result_output.append("".join(parts))

    def print_plain_matrix(self, title, matrix):
        """Выводит обычную матрицу без вектора B."""
        parts = [
            '<div style="margin:8px 0 18px 0;">',
            '<div style="font-size:32px;font-weight:bold;'
            'color:#713650;margin-bottom:8px;">',
            html.escape(title),
            '</div>',
            '<table style="border-collapse:collapse;'
            'font-family:Consolas,monospace;font-size:30px;">'
        ]

        for row in matrix:
            parts.append("<tr>")

            for value in row:
                parts.append(
                    '<td style="padding:5px 9px;'
                    'text-align:right;white-space:nowrap;">'
                    f'{value:.10f}</td>'
                )

            parts.append("</tr>")

        parts.append("</table></div>")
        self.result_output.append("".join(parts))

    @staticmethod
    def _fmt(value):
        """Форматирует число, корректно обрабатывая inf/nan."""
        if value is None:
            return "—"
        if isinstance(value, float):
            if math.isnan(value):
                return "nan"
            if math.isinf(value):
                return "inf" if value > 0 else "-inf"
        return f"{value:.10f}"

    def print_iteration_table(self, result):
        """Выводит таблицу итераций."""
        n = len(result["X"])

        table = [
            '<table style="border-collapse:collapse;'
            'font-family:Consolas,monospace;font-size:28px;">',
            '<tr style="background:#f8dce8;">'
            '<th style="padding:6px 10px;">k</th>'
            + ''.join(
                f'<th style="padding:6px 10px;">x{i + 1}</th>'
                for i in range(n)
            )
            + '<th style="padding:6px 10px;">δ</th></tr>'
        ]

        for step in result["iterations"]:
            delta_text = self._fmt(step["delta"])

            table.append(
                f'<tr><td style="padding:5px 10px;">'
                f'{step["k"]}</td>'
                + ''.join(
                    '<td style="padding:5px 10px;text-align:right;">'
                    f'{self._fmt(value)}</td>'
                    for value in step["x"]
                )
                + '<td style="padding:5px 10px;text-align:right;">'
                f'{delta_text}</td></tr>'
            )

        table.append("</table>")
        self.result_output.append("".join(table))

    def print_iteration_summary(self, result, title):
        """Выводит решение, количество итераций и статус сходимости."""
        self.result_output.append(
            '<h3 style="color:#713650;margin-top:18px;font-size:32px;">'
            f'{html.escape(title)}</h3>'
        )

        for i, value in enumerate(result["X"]):
            self.result_output.append(
                '<div style="padding:4px 10px;color:#713650;">'
                f'<b>x{i + 1} = {self._fmt(value)}</b></div>'
            )

        self.result_output.append(
            '<div style="padding:4px 10px;">'
            f'Выполнено итераций: {result["k"]}<br>'
            f'Итоговая δ = {self._fmt(result["delta"])}</div>'
        )

        if result["converged"]:
            self.print_action(
                "Условие δ ≤ ε выполнено. Точность достигнута."
            )
        else:
            self.print_action(
                "Достигнут лимит итераций. "
                "Заданная точность не достигнута."
            )

    def print_action(self, action):
        """Выводит описание действия."""
        self.result_output.append(
            '<div style="background:#fff0f6;'
            'border-left:4px solid #d779a0;'
            'padding:7px 10px;margin:10px 0;color:#713650;'
            'white-space:normal;">'
            f'<b>Действие:</b> {html.escape(str(action))}</div>'
        )

    def print_reference_block(self, reference, comparison, x_current, method_name):
        """Выводит эталонное решение и сравнение с ним в HTML."""
        if reference is None:
            return

        self.result_output.append(
            '<h3 style="color:#713650;margin-top:18px;font-size:32px;">'
            f'Эталонное решение (numpy.linalg.solve) — {html.escape(method_name)}'
            '</h3>'
        )

        if reference["X"] is None:
            self.result_output.append(
                '<div style="padding:4px 10px;color:#a01040;">'
                f'Ошибка эталона: {html.escape(reference["error"])}'
                '</div>'
            )
            return

        for i, value in enumerate(reference["X"]):
            self.result_output.append(
                '<div style="padding:3px 10px;">'
                f'x{i + 1}* = {value:.12f}</div>'
            )

        if comparison is not None:
            self.result_output.append(
                '<h4 style="color:#713650;margin-top:12px;font-size:30px;">'
                f'Сравнение с эталоном ({html.escape(method_name)})</h4>'
            )

            for i, (xi, xr, d) in enumerate(
                zip(x_current, reference["X"], comparison["diff"]), start=1
            ):
                self.result_output.append(
                    '<div style="padding:2px 10px;">'
                    f'x{i}: метод = {xi:.10f} | '
                    f'эталон = {xr:.10f} | '
                )


    def solve(self):
        """Запускает выбранный метод."""
        self.result_output.clear()

        try:
            A, B = self.parse_equations()

            self.result_output.append(
                '<h3 style="color:#713650;font-size:32px;">'
                'Исходная система</h3>'
            )

            self.print_matrix(
                "Расширенная матрица [A | B]",
                A,
                B
            )

            # Метод Гаусса — Краута остаётся отдельным вариантом.
            if self.method_choice.currentIndex() == 0:
                X, residuals, steps, reference, comparison = gauss_krauth(
                    [row[:] for row in A],
                    B[:]
                )

                for title, matrix, vector, action in steps[1:]:
                    self.print_action(action)
                    self.print_matrix(title, matrix, vector)

                self.result_output.append(
                    '<h3 style="color:#713650;margin-top:18px;font-size:32px;">'
                    'Решение системы методом Гаусса — Краута</h3>'
                )

                for i, value in enumerate(X):
                    self.result_output.append(
                        '<div style="padding:4px 10px;color:#713650;">'
                        f'<b>x{i + 1} = {value:.10f}</b></div>'
                    )

                self.result_output.append(
                    '<h3 style="color:#713650;margin-top:18px;font-size:32px;">'
                    'Невязки по исходной системе</h3>'
                )

                for i, value in enumerate(residuals):
                    shown = 0.0 if abs(value) < 1e-10 else value
                    self.result_output.append(
                        f'<div style="padding:3px 10px;">'
                        f'r{i + 1} = {shown:.10f}</div>'
                    )

                self.print_reference_block(
                    reference,
                    comparison,
                    X,
                    "Метод Гаусса — Краута"
                )

                return

            # Проверяем параметры итерационных методов.
            epsilon_text = (
                self.epsilon_input.text()
                .strip()
                .replace(",", ".")
            )

            try:
                epsilon = float(epsilon_text)
            except ValueError as error:
                raise ValueError(
                    "Введите корректную точность ε, например 0,000001."
                ) from error

            if not math.isfinite(epsilon) or epsilon <= 0:
                raise ValueError(
                    "Точность ε должна быть положительным "
                    "конечным числом."
                )

            kmax = self.max_iterations_input.value()

            # -------------------------------------------------
            # ДЕМОНСТРАЦИЯ: метод простых итераций на исходной
            # матрице, без преобразования. Обычно расходится.
            # -------------------------------------------------
            self.result_output.append(
                '<hr><h2 style="color:#713650;font-size:34px;">'
                '0. Демонстрация: метод простых итераций '
                'на исходной матрице (без преобразования)</h2>'
            )

            try:
                result_raw = simple_iterations_raw(
                    [row[:] for row in A],
                    B[:],
                    epsilon,
                    min(kmax, 30)  # ограничим вывод, чтобы таблица не была огромной
                )

                self.result_output.append(
                    '<h3 style="color:#713650;font-size:32px;">'
                    'Таблица итераций на исходной матрице</h3>'
                )
                self.print_iteration_table(result_raw)

                last = result_raw["iterations"][-1]
                last_x = last["x"]

                has_inf = any(
                    isinstance(v, float) and not math.isfinite(v)
                    for v in last_x
                )

                self.result_output.append(
                    '<div style="padding:4px 10px;">'
                    f'Итераций выполнено: {result_raw["k"]}<br>'
                    f'Последняя δ = {self._fmt(last["delta"])}<br>'
                    f'Сошлось: '
                    f'{"да" if result_raw["converged"] else "нет"}'
                    '</div>'
                )

                if not result_raw["converged"]:
                    if has_inf:
                        self.print_action(
                            "На исходной матрице без преобразования метод "
                            "разошёлся: компоненты решения выросли до "
                            "бесконечности (inf), условие диагонального "
                            "преобладания не выполнено."
                        )
                    else:
                        self.print_action(
                            "На исходной матрице без преобразования метод "
                            "не сошёлся за отведённое число итераций: "
                            "числа лавинообразно растут, условие "
                            "диагонального преобладания не выполнено."
                        )

            except (
                ValueError,
                TypeError,
                OverflowError,
                ZeroDivisionError
            ) as error:
                self.print_action(
                    f"Демонстрация на исходной матрице: {error}"
                )

            # -------------------------------------------------
            # ПЕРВОЕ РЕШЕНИЕ: существующий метод.
            # Система приводится к диагональному преобладанию.
            # -------------------------------------------------
            self.result_output.append(
                '<h2 style="color:#713650;font-size:34px;">'
                '1. Обычный метод простых итераций</h2>'
            )

            try:
                result1 = simple_iterations(
                    [row[:] for row in A],
                    B[:],
                    epsilon,
                    kmax
                )

                self.result_output.append(
                    '<h3 style="color:#713650;font-size:32px;">'
                    'Преобразование системы к '
                    'диагональному преобладанию</h3>'
                )

                for action in result1["transformations"]:
                    self.print_action(action)

                self.print_matrix(
                    "Преобразованная расширенная матрица [A | B]",
                    result1["A"],
                    result1["B"]
                )

                self.result_output.append(
                    '<h3 style="color:#713650;font-size:32px;">'
                    'Таблица итераций первого метода</h3>'
                )

                self.print_iteration_table(result1)

                self.print_iteration_summary(
                    result1,
                    "Результат первого метода"
                )

                residual1 = [
                    sum(
                        A[i][j] * result1["X"][j]
                        for j in range(len(B))
                    ) - B[i]
                    for i in range(len(B))
                ]

                self.result_output.append(
                    '<h3 style="color:#713650;font-size:32px;">'
                    'Невязка первого решения по исходной системе '
                    'AX = B</h3>'
                )

                for i, value in enumerate(residual1):
                    shown = 0.0 if abs(value) < 1e-10 else value
                    self.result_output.append(
                        f'<div style="padding:3px 10px;">'
                        f'R{i + 1} = {shown:.10f}</div>'
                    )

                self.print_reference_block(
                    result1.get("reference"),
                    result1.get("comparison"),
                    result1["X"],
                    "Метод простых итераций"
                )

            except (
                ValueError,
                TypeError,
                OverflowError,
                ZeroDivisionError
            ) as error:
                self.print_action(
                    f"Первый метод не удалось выполнить: {error}"
                )

            # -------------------------------------------------
            # ВТОРОЕ РЕШЕНИЕ: C = A^T * A, D = A^T * B.
            # -------------------------------------------------
            self.result_output.append(
                '<hr><h2 style="color:#713650;font-size:34px;">'
                '2. Метод простых итераций через '
                'транспонированную матрицу</h2>'
            )

            try:
                result2 = simple_iterations_transposed(
                    [row[:] for row in A],
                    B[:],
                    epsilon,
                    kmax
                )

                self.print_plain_matrix(
                    "Транспонированная матрица Aᵀ",
                    result2["AT"]
                )

                self.print_matrix(
                    "Матрица C = AᵀA и вектор D = AᵀB",
                    result2["C"],
                    result2["D"]
                )

                self.result_output.append(
                    '<h3 style="color:#713650;font-size:32px;">'
                    'Таблица итераций второго метода для CX = D'
                    '</h3>'
                )

                self.print_iteration_table(result2)

                self.print_iteration_summary(
                    result2,
                    "Результат второго метода"
                )

                self.result_output.append(
                    '<h3 style="color:#713650;margin-top:18px;font-size:32px;">'
                    'Невязка второго решения по исходной системе '
                    'AX = B</h3>'
                )

                for i, value in enumerate(result2["residual"]):
                    shown = 0.0 if abs(value) < 1e-10 else value
                    self.result_output.append(
                        f'<div style="padding:3px 10px;">'
                        f'R{i + 1} = {shown:.10f}</div>'
                    )

                self.result_output.append(
                    '<div style="padding:4px 10px;">'
                    '<b>Норма невязки: '
                    f'{result2["residual_norm"]:.10f}</b></div>'
                )

                self.print_reference_block(
                    result2.get("reference"),
                    result2.get("comparison"),
                    result2["X"],
                    "Метод простых итераций (AᵀA)"
                )

            except (
                ValueError,
                TypeError,
                OverflowError,
                ZeroDivisionError
            ) as error:
                self.print_action(
                    f"Второй метод не удалось выполнить: {error}"
                )

        except (
            ValueError,
            TypeError,
            OverflowError,
            ZeroDivisionError
        ) as error:
            QMessageBox.warning(self, "Ошибка", str(error))

    def clear_result(self):
        """Очищает область результатов."""
        self.result_output.clear()