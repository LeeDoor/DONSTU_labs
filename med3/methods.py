import numpy as np
import math
from scipy.optimize import root

def get_reference_solution():
    def f_vec(v): return F(v[0], v[1])
    sol = root(f_vec, [-0.2, 0.5], method='hybr', tol=1e-14)
    return (float(sol.x[0]), float(sol.x[1])) if sol.success else (0.0, 0.0)

def F(x, y):
    return np.array([
        math.sin(y + 1.0) - x - 1.2,
        2.0 * y + math.cos(x) - 2.0
    ], dtype=float)

def Jacobi(x, y):
    return np.array([
        [-1.0, math.cos(y + 1.0)],
        [-math.sin(x), 2.0]
    ], dtype=float)

def gauss_solve_with_log(A, b):
    n = len(b)
    M = [list(map(float, row)) for row in A]
    rhs = list(map(float, b))
    log = []

    log.append("  Исходная матрица [W | -F]:")
    for i in range(n):
        log.append(f"    [ {M[i][0]:>8.4f}  {M[i][1]:>8.4f} | {rhs[i]:>8.4f} ]")

    log.append("  Прямой ход:")
    for k in range(n):
        max_row = k
        for i in range(k + 1, n):
            if abs(M[i][k]) > abs(M[max_row][k]): max_row = i

        if max_row != k:
            M[k], M[max_row] = M[max_row], M[k]
            rhs[k], rhs[max_row] = rhs[max_row], rhs[k]
            log.append(f"    Поменяли строки {k + 1} и {max_row + 1}:")
            for i in range(n):
                log.append(f"    [ {M[i][0]:>8.4f}  {M[i][1]:>8.4f} | {rhs[i]:>8.4f} ]")

        for i in range(k + 1, n):
            factor = M[i][k] / M[k][k]
            for j in range(k, n): M[i][j] -= factor * M[k][j]
            rhs[i] -= factor * rhs[k]
            log.append(f"    Из строки {i + 1} вычли строку {k + 1} * {factor:.4f}:")
            for r in range(n):
                log.append(f"    [ {M[r][0]:>8.4f}  {M[r][1]:>8.4f} | {rhs[r]:>8.4f} ]")

    log.append("  Обратный ход:")
    x = [0.0] * n
    for i in range(n - 1, -1, -1):
        s = sum(M[i][j] * x[j] for j in range(i + 1, n))
        x[i] = (rhs[i] - s) / M[i][i]
        log.append(f"    x{i + 1} = ({rhs[i]:.4f} - ({s:.4f})) / {M[i][i]:.4f} = {x[i]:.4f}")

    log.append(f"  Получили поправки: ΔX = ({x[0]:.6f}, {x[1]:.6f})")
    return np.array(x), log

def newton_method(x0, y0, eps=1e-5, max_iter=100):
    x, y = float(x0), float(y0)
    history, gauss_logs = [], []
    reason = "max_iter"

    for k in range(1, max_iter + 1):
        Fk = F(x, y)
        Wk = Jacobi(x, y)
        det = Wk[0, 0] * Wk[1, 1] - Wk[0, 1] * Wk[1, 0]
        if abs(det) < 1e-15:
            raise ValueError(f"Матрица Якоби вырождена на шаге {k}")

        dX, log_lines = gauss_solve_with_log(Wk, -Fk)
        gauss_logs.append((k, log_lines))

        dx, dy = float(dX[0]), float(dX[1])
        x_new, y_new = x + dx, y + dy

        # d = float(np.max(np.abs(dX)))
        # d = float(np.sum(np.abs(dX)) / len(dX))
        d = float(np.sqrt(np.sum(np.abs(dX) ** 2)))
        fx, fy = F(x_new, y_new)
        history.append((k, x_new, y_new, dx, dy, d, fx, fy))

        x, y = x_new, y_new
        if d <= eps:
            reason = "converged"
            break
    return x, y, len(history), history, gauss_logs, reason

def phi1(x, y): return math.sin(y + 1.0) - 1.2
def phi2(x, y): return 1.0 - math.cos(x) / 2.0

def iter_method(x0, y0, eps=1e-5, max_iter=1000, method_type="simple"):
    x, y = float(x0), float(y0)
    history = []
    reason = "max_iter"

    dphi1_dy = math.cos(y0 + 1.0)
    dphi2_dx = math.sin(x0) / 2.0
    conv_eq1 = abs(dphi1_dy) < 1.0
    conv_eq2 = abs(dphi2_dx) < 1.0
    conv_ok = conv_eq1 and conv_eq2

    for k in range(1, max_iter + 1):
        x_old, y_old = x, y

        if method_type == "seidel":
            x = phi1(x_old, y_old)
            y = phi2(x, y_old)
        else:
            x = phi1(x_old, y_old)
            y = phi2(x_old, y_old)

        dx, dy = x - x_old, y - y_old
        dX = np.array([dx, dy])

        # d = float(np.max(np.abs(dX)))
        # d = float(np.sum(np.abs(dX)) / len(dX))
        d = float(np.sqrt(np.sum(np.array(dX) ** 2)))
        fx, fy = F(x, y)
        history.append((k, x, y, dx, dy, d, fx, fy))

        if d <= eps:
            reason = "converged"
            break
        if not (math.isfinite(x) and math.isfinite(y)) or abs(x) > 1e6 or abs(y) > 1e6:
            reason = "diverged"
            break

    return x, y, len(history), history, conv_ok, reason, conv_eq1, conv_eq2
