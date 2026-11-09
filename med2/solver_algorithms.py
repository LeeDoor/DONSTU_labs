import math

try:
    import numpy as np
except ImportError:
    np = None


DECIMAL_PLACES = 12


def fmt(value):
    if not math.isfinite(value):
        return str(value)
    return f"{round(value, DECIMAL_PLACES):.{DECIMAL_PLACES}f}"


def norm_inf(values):
    return max((abs(value) for value in values), default=0.0)


def format_matrix(matrix, rhs=None):
    lines = []

    for i, row in enumerate(matrix):
        values = "  ".join(
            f"{fmt(value):>{DECIMAL_PLACES + 8}}"
            for value in row
        )

        if rhs is not None:
            values += f"  |  {fmt(rhs[i]):>{DECIMAL_PLACES + 8}}"

        lines.append(values)

    return "\n".join(lines)


def matching_decimal_places(first, second, max_places=12):
    for places in range(max_places, -1, -1):
        if all(
            round(a, places) == round(b, places)
            for a, b in zip(first, second)
        ):
            return places

    return 0


def numpy_comparison(a, b, x):
    lines = ["ПРОВЕРКА С ПОМОЩЬЮ NUMPY"]

    if np is None:
        lines.extend([
            "NumPy не установлен. Выполните:",
            "python -m pip install numpy"
        ])
        return "\n".join(lines)

    try:
        x_np = np.linalg.solve(
            np.array(a, dtype=float),
            np.array(b, dtype=float)
        ).tolist()

    except np.linalg.LinAlgError:
        return (
            "ПРОВЕРКА NUMPY: система вырождена; "
            "единственное решение не найдено."
        )

    error = norm_inf([
        x[i] - x_np[i]
        for i in range(len(x))
    ])

    places = matching_decimal_places(
        x, x_np, DECIMAL_PLACES
    )

    lines.extend([
        "Решение numpy.linalg.solve:",
        *[
            f"x{i + 1} = {fmt(value)}"
            for i, value in enumerate(x_np)
        ],
        "",
        (
            "Совпавших знаков после запятой "
            f"во всех компонентах: {places}."
        ),
        (
            "Максимальная абсолютная погрешность "
            f"относительно NumPy: {fmt(error)}."
        )
    ])

    return "\n".join(lines)


def gauss_jordan(a, b, tol):
    n = len(a)

    m = [
        list(map(float, row)) + [float(b[i])]
        for i, row in enumerate(a)
    ]

    log = [
        "МЕТОД ГАУССА — ЖОРДАНА",
        "",
        "Исходная расширенная матрица:",
        format_matrix(a, b),
        "",
        "РЕЗУЛЬТАТ ПРЯМОГО ХОДА:",
    ]

    for k in range(n):
        pivot_row = max(
            range(k, n),
            key=lambda i: abs(m[i][k])
        )

        if abs(m[pivot_row][k]) <= 1e-15:
            raise ValueError(
                "Матрица вырождена или ведущий элемент "
                "слишком близок к нулю."
            )

        if pivot_row != k:
            m[k], m[pivot_row] = m[pivot_row], m[k]

        pivot = m[k][k]

        for j in range(k, n + 1):
            m[k][j] /= pivot

        for i in range(k + 1, n):
            factor = m[i][k]

            for j in range(k, n + 1):
                m[i][j] -= factor * m[k][j]

    log.extend([
        format_matrix(
            [row[:n] for row in m],
            [row[n] for row in m]
        ),
        "",
        "РЕЗУЛЬТАТ ОБРАТНОГО ХОДА:",
    ])

    for k in range(n - 1, -1, -1):
        for i in range(k):
            factor = m[i][k]

            for j in range(k, n + 1):
                m[i][j] -= factor * m[k][j]

    x = [m[i][n] for i in range(n)]

    log.extend([
        format_matrix(
            [row[:n] for row in m],
            [row[n] for row in m]
        ),
        "",
        "ОТВЕТ:",
        *[
            f"x{i + 1} = {fmt(value)}"
            for i, value in enumerate(x)
        ],
        "",
        numpy_comparison(a, b, x)
    ])

    return x, "\n".join(log)


def simple_iteration(a, b, tol, max_iter):
    n = len(a)

    normal_a = [
        [
            sum(a[k][i] * a[k][j] for k in range(n))
            for j in range(n)
        ]
        for i in range(n)
    ]
    normal_b = [
        sum(a[k][i] * b[k] for k in range(n))
        for i in range(n)
    ]

    log = [
        "МЕТОД ПРОСТЫХ ИТЕРАЦИЙ",
        "",
        "ИСХОДНАЯ СИСТЕМА:",
        format_matrix(a, b),
        "",
        "НОРМАЛЬНАЯ СИСТЕМА AᵀA x = Aᵀb:",
        format_matrix(normal_a, normal_b),
    ]

    if not all(
        math.isfinite(value)
        for row in normal_a
        for value in row
    ) or not all(math.isfinite(value) for value in normal_b):
        raise ValueError(
            "Не удалось сформировать нормальную систему: "
            "переполнение при вычислении AᵀA или Aᵀb."
        )

    scale = max(abs(normal_a[i][i]) for i in range(n))
    lower = [[0.0] * n for _ in range(n)]

    for i in range(n):
        for j in range(i + 1):
            value = normal_a[i][j] - sum(
                lower[i][k] * lower[j][k]
                for k in range(j)
            )

            if i == j:
                if value <= scale * 1e-14:
                    raise ValueError(
                        "Матрица A вырождена или плохо обусловлена; "
                        "нормальная система не имеет надежного "
                        "единственного решения."
                    )
                lower[i][j] = math.sqrt(value)
            else:
                lower[i][j] = value / lower[j][j]

    spectral_bound = max(
        sum(
            abs(normal_a[i][j])
            / (math.sqrt(normal_a[i][i]) * math.sqrt(normal_a[j][j]))
            for j in range(n)
        )
        for i in range(n)
    )
    relaxation = 1.0 / spectral_bound

    log.extend([
        "",
        "Решение нормальной системы методом Якоби с релаксацией.",
        "",
        "Начальное приближение:"
    ])

    x = [normal_b[i] / normal_a[i][i] for i in range(n)]

    log.extend([
        *[
            f"x{i + 1}(0) = {fmt(x[i])}"
            for i in range(n)
        ],
        "",
        "ТАБЛИЦА ИТЕРАЦИЙ:"
    ])

    headers = (
        ["k"]
        + [f"x{i + 1}" for i in range(n)]
        + [f"d{i + 1}" for i in range(n)]
    )

    widths = [8] + [28] * (2 * n)

    log.append(
        "".join(
            headers[i].ljust(widths[i])
            for i in range(len(headers))
        )
    )

    log.append("-" * sum(widths))

    converged = False
    differences = [float("inf")] * n
    k = 0

    for k in range(1, max_iter + 1):
        new_x = [
            x[i] + relaxation * (
                normal_b[i]
                - sum(normal_a[i][j] * x[j] for j in range(n))
            ) / normal_a[i][i]
            for i in range(n)
        ]

        for value in new_x:
            if not math.isfinite(value):
                raise ValueError(
                    "Итерационный процесс стал неустойчивым. "
                    "Проверьте обусловленность исходной матрицы."
                )

        differences = []

        for i in range(n):
            difference = abs(new_x[i] - x[i])
            relative_difference = difference / max(
                abs(new_x[i]), 1.0
            )
            differences.append(relative_difference)

        values = (
            [str(k)]
            + [fmt(value) for value in new_x]
            + [fmt(value) for value in differences]
        )

        log.append(
            "".join(
                values[i].ljust(widths[i])
                for i in range(len(values))
            )
        )

        x = new_x

        if all(value < tol for value in differences):
            converged = True
            break

    if not converged:
        raise ValueError(
            f"Метод не сошелся за {max_iter} итераций. "
            "Увеличьте максимальное число итераций."
        )

    log.extend([
        "",
        f"Итераций выполнено: {k}.",
        "",
        "ОТВЕТ:",
        *[
            f"x{i + 1} = {fmt(value)}"
            for i, value in enumerate(x)
        ],
        "",
        numpy_comparison(a, b, x)
    ])

    return x, "\n".join(log)
