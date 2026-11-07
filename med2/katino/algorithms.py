import math
from itertools import product
from transformations import transform_system
from need import (rank, validate_system, validate_iterations, reference_solution)


def gauss_krauth(A, B):
    n = validate_system(A, B)
    A0 = [row[:] for row in A]
    B0 = B[:]

    steps = []
    def save_step(title, action):
        steps.append((title, [row[:] for row in A], B[:], action))
    save_step("Начальная матрица", "Исходная расширенная матрица [A | B]")

    # прямой ход
    for k in range(n - 1):
        # выбор ведущего элемента по максимальному модулю
        m = max(range(k, n), key=lambda i: abs(A[i][k]))

        if A[m][k] == 0:
            raise ValueError("Матрица вырождена: единственного решения нет.")

        if m != k:
            A[k], A[m] = A[m], A[k]
            B[k], B[m] = B[m], B[k]
            save_step("Перестановка строк", f"R{k + 1} <-> R{m + 1}")

        # обнуляем элементы под ведущим элементом
        for i in range(k + 1, n):
            q = A[i][k] / A[k][k]
            A[i][k] = 0.0

            for j in range(k + 1, n):
                A[i][j] -= q * A[k][j]

            B[i] -= q * B[k]
            save_step(f"Преобразование строки {i + 1}",f"R{i + 1} <- R{i + 1} - ({q:.10f}) * R{k + 1}")

    if A[n - 1][n - 1] == 0:
        raise ValueError("Матрица вырождена: последний диагональный элемент равен нулю.")

    save_step("Конечная матрица после прямого хода","Получена верхняя треугольная матрица.")

    # обратный ход
    X = [0.0] * n

    for i in range(n - 1, -1, -1):
        if A[i][i] == 0:
            raise ValueError("Нулевой диагональный элемент.")

        s = sum(A[i][j] * X[j] for j in range(i + 1, n))

        X[i] = (B[i] - s) / A[i][i]

    if not all(math.isfinite(x) for x in X):
        raise ValueError("Получено недопустимое решение.")

    # невязки
    Delta = []
    for i in range(n):
        calculated = sum(A0[i][j] * X[j] for j in range(n))
        Delta.append(B0[i] - calculated)

    # эталон и сравнение
    reference = reference_solution(A0, B0)

    comparison = None
    if reference["X"] is not None:
        diff = [X[i] - reference["X"][i] for i in range(n)]
        comparison = {
            "diff": diff,
            "diff_norm": math.sqrt(sum(d * d for d in diff)),
        }

    return X, Delta, steps, reference, comparison


def simple_iterations(A, B, epsilon, kmax):
    #ввод
    n = validate_iterations(A, B, epsilon, kmax)
    A0 = [list(map(float, row)) for row in A]
    B0 = list(map(float, B))
    # преобразование системы в диагональное преобладание
    A_work, B_work, transformations = transform_system(A0, B0)

    #Xi := 0, i = 1...n
    X = [0.0] * n

    #к:=0
    iterations = [{"k": 0, "x": X[:], "delta": None}]

    delta = math.inf
    converged = False

    # k := k + 1
    for k in range(1, kmax + 1):
        #X0i := Xi, i = 1...n
        X_old = X[:]
        X_new = [0.0] * n

        #МПИ
        for i in range(n):
            #s = sum(A_work[i][j] * X_old[j] for j in range(n) if j != i) для МПИ
            s = sum(A_work[i][j] * (X_new[j] if j < i else X_old[j]) for j in range(n) if j != i) #lkz ptqltkz

            X_new[i] = (B_work[i] - s) / A_work[i][i]

        if not all(math.isfinite(v) for v in X_new):
            raise ValueError("В процессе итераций получены недопустимые числа")

        # дельта через корень
        delta = math.sqrt(sum((X_new[i] - X_old[i]) ** 2 for i in range(n)))

        X = X_new

        iterations.append({"k": k, "x": X[:], "delta": delta})

        if delta <= epsilon:
            converged = True
            break

    # эталон и сравнение
    reference = reference_solution(A0, B0)

    comparison = None
    if reference["X"] is not None:
        diff = [X[i] - reference["X"][i] for i in range(n)]
        comparison = {"diff": diff, "diff_norm": math.sqrt(sum(d * d for d in diff))}

    return {
        "original_A": A0,
        "original_B": B0,
        "A": A_work,
        "B": B_work,
        "transformations": transformations,
        "iterations": iterations,
        "X": X,
        "delta": delta,
        "epsilon": epsilon,
        "k": len(iterations) - 1,
        "kmax": kmax,
        "converged": converged,
        "reference": reference,
        "comparison": comparison,
    }


def simple_iterations_transposed(A, B, epsilon, kmax):
    n = validate_iterations(A, B, epsilon, kmax)
    A0 = [list(map(float, row)) for row in A]
    B0 = list(map(float, B))

    # транспонируем исходную матрицу A
    AT = [
        [A0[j][i] for j in range(n)]
        for i in range(n)
    ]

    # C = A^T * A
    C = [
        [
            sum(AT[i][k] * A0[k][j] for k in range(n))
            for j in range(n)
        ]
        for i in range(n)
    ]

    # D = A^T * B
    D = [
        sum(AT[i][k] * B0[k] for k in range(n))
        for i in range(n)
    ]

    for i in range(n):
        if abs(C[i][i]) < 1e-15:
            raise ValueError("Нулевой диагональный элемент матрицы C. Метод простых итераций применить нельзя.")

    X = [0.0] * n
    iterations = [{"k": 0, "x": X[:], "delta": None}]

    delta = math.inf
    converged = False

    for iteration in range(1, kmax + 1):
        X_old = X[:]
        X_new = [0.0] * n

        for i in range(n):
            #s = sum(C[i][j] * X_old[j] for j in range(n) if j != i) простых
            s = sum(C[i][j] * (X_new[j] if j < i else X_old[j]) for j in range(n) if j != i) #зейдель

            X_new[i] = (D[i] - s) / C[i][i]

        if not all(math.isfinite(x) for x in X_new):
            raise ValueError("В процессе итераций получены недопустимые числа.")

        delta = math.sqrt(sum((X_new[i] - X_old[i]) ** 2 for i in range(n)))

        X = X_new
        iterations.append({
            "k": iteration,
            "x": X[:],
            "delta": delta
        })

        if delta <= epsilon:
            converged = True
            break

    # невязки
    residual = [sum(A0[i][j] * X[j] for j in range(n)) - B0[i] for i in range(n)]
    residual_norm = math.sqrt(sum(value ** 2 for value in residual))

    # эталон и сравнение
    reference = reference_solution(A0, B0)

    comparison = None
    if reference["X"] is not None:
        diff = [X[i] - reference["X"][i] for i in range(n)]
        comparison = {
            "diff": diff,
            "diff_norm": math.sqrt(sum(d * d for d in diff)),
        }

    return {
        "original_A": A0,
        "original_B": B0,
        "AT": AT,
        "C": C,
        "D": D,
        "iterations": iterations,
        "X": X,
        "delta": delta,
        "epsilon": epsilon,
        "k": len(iterations) - 1,
        "kmax": kmax,
        "converged": converged,
        "residual": residual,
        "residual_norm": residual_norm,
        "reference": reference,
        "comparison": comparison,
    }

#s = sum(A_work[i][j] * (X_new[j] if j < i else X_old[j]) for j in range(n) if j != i)
#s = sum(C[i][j] * (X_new[j] if j < i else X_old[j]) for j in range(n) if j != i)




def simple_iterations_raw(A, B, epsilon, kmax):
    n = validate_iterations(A, B, epsilon, kmax)

    A0 = [list(map(float, row)) for row in A]
    B0 = list(map(float, B))

    X = [0.0] * n
    iterations = [{"k": 0, "x": X[:], "delta": None}]

    delta = math.inf
    converged = False

    for k in range(1, kmax + 1):
        X_old = X[:]
        X_new = [0.0] * n
        diverged = False

        for i in range(n):
            if abs(A0[i][i]) < 1e-15:
                raise ValueError(f"Нулевой диагональный элемент A[{i + 1}][{i + 1}] — метод простых итераций в исходном виде применить нельзя.")
            try:
                s = sum(A0[i][j] * X_old[j] for j in range(n) if j != i)
                #s = sum(A_work[i][j] * (X_new[j] if j < i else X_old[j]) for j in range(n) if j != i)

                X_new[i] = (B0[i] - s) / A0[i][i]
            except (OverflowError, ZeroDivisionError):
                X_new[i] = math.inf
                diverged = True
                break

            if not math.isfinite(X_new[i]):
                diverged = True
                break

        if diverged or not all(math.isfinite(v) for v in X_new):
            # фиксируем последний шаг с inf и выходим
            safe_row = [
                (v if isinstance(v, float) and math.isfinite(v) else math.inf)
                for v in X_new
            ]
            delta = math.inf
            X = safe_row
            iterations.append({"k": k, "x": X[:], "delta": delta})
            break

        delta = math.sqrt(sum((X_new[i] - X_old[i]) ** 2 for i in range(n)))
        X = X_new
        iterations.append({"k": k, "x": X[:], "delta": delta})

        if delta <= epsilon:
            converged = True
            break

    reference = reference_solution(A0, B0)
    comparison = None
    if reference["X"] is not None:
        diff = [X[i] - reference["X"][i] for i in range(n)]
        comparison = {
            "diff": diff,
            "diff_norm": math.sqrt(sum(d * d for d in diff)),
        }

    return {
        "original_A": A0,
        "original_B": B0,
        "iterations": iterations,
        "X": X,
        "delta": delta,
        "epsilon": epsilon,
        "k": len(iterations) - 1,
        "kmax": kmax,
        "converged": converged,
        "reference": reference,
        "comparison": comparison,
    }