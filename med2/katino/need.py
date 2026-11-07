import math

import numpy as np


def rank(matrix):
    """Вычисляет ранг матрицы методом Гаусса."""
    M = [row[:] for row in matrix]

    rows = len(M)
    cols = len(M[0]) if rows else 0

    if not rows:
        return 0

    scale = max(
        (abs(v) for row in M for v in row),
        default=1.0
    )
    tol = 1e-10 * max(1.0, scale)

    r = 0

    for col in range(cols):
        if r >= rows:
            break

        pivot = max(
            range(r, rows),
            key=lambda i: abs(M[i][col])
        )

        if abs(M[pivot][col]) <= tol:
            continue

        M[r], M[pivot] = M[pivot], M[r]
        pivot_value = M[r][col]

        for i in range(r + 1, rows):
            factor = M[i][col] / pivot_value

            for j in range(col, cols):
                M[i][j] -= factor * M[r][j]

        r += 1

    return r


def validate_system(A, B):
    """Проверяет размеры матрицы и корректность чисел."""
    n = len(B)

    if n == 0 or len(A) != n:
        raise ValueError("Матрица должна быть квадратной.")

    if any(len(row) != n for row in A):
        raise ValueError("Матрица должна быть квадратной.")

    if not all(math.isfinite(v) for row in A for v in row):
        raise ValueError("Матрица содержит недопустимые числа.")

    if not all(math.isfinite(v) for v in B):
        raise ValueError("Вектор B содержит недопустимые числа.")

    return n


def validate_iterations(A, B, epsilon, kmax):
    """Проверяет систему и параметры метода итераций."""
    n = validate_system(A, B)

    if not math.isfinite(epsilon) or epsilon <= 0:
        raise ValueError(
            "Точность ε должна быть положительным числом."
        )

    if (
        not isinstance(kmax, int)
        or isinstance(kmax, bool)
        or kmax < 1
    ):
        raise ValueError(
            "kmax должен быть целым числом не меньше 1."
        )

    return n


def reference_solution(A, B):
    """Эталонное решение системы AX = B через numpy.linalg.solve."""
    try:
        A_np = np.array(A, dtype=float)
        B_np = np.array(B, dtype=float)
        X_ref = np.linalg.solve(A_np, B_np)

        residual = A_np @ X_ref - B_np
        residual_norm = float(np.linalg.norm(residual))

        return {
            "X": X_ref.tolist(),
            "residual": residual.tolist(),
            "residual_norm": residual_norm,
            "error": None,
        }
    except np.linalg.LinAlgError as exc:
        return {
            "X": None,
            "residual": None,
            "residual_norm": None,
            "error": f"numpy.linalg.solve: {exc}",
        }


def format_reference_block(reference, comparison, x_current, method_name):
    """Формирует текстовый блок с эталоном и сравнением."""
    if reference is None:
        return ""
    if reference["X"] is None:
        return (
            "\n=== Эталон (numpy.linalg.solve) ===\n"
            f"Ошибка: {reference['error']}\n"
        )

    lines = ["\n=== Эталонное решение (numpy.linalg.solve) ==="]
    for i, v in enumerate(reference["X"], start=1):
        lines.append(f"x{i}* = {v:.10f}")

    lines.append(
        f"\nНевязка эталона ||A·X* - B|| = {reference['residual_norm']:.3e}"
    )

    if comparison is not None:
        lines.append(f"\n=== Сравнение ({method_name}) с эталоном ===")
        for i, (xi, xr, d) in enumerate(
            zip(x_current, reference["X"], comparison["diff"]), start=1
        ):
            lines.append(
                f"x{i}: {method_name} = {xi:.10f} | "
                f"эталон = {xr:.10f} | "
                f"разность = {d:+.3e}"
            )
        lines.append(
            f"\n||X_{method_name} - X*|| = {comparison['diff_norm']:.3e}"
        )

    return "\n".join(lines) + "\n"