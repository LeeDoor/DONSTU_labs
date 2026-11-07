from itertools import product

from need import rank, validate_system


def transform_system(A, B):
    n = validate_system(A, B)

    A0 = [list(map(float, row)) for row in A]
    B0 = list(map(float, B))

    coefficient_range = range(-3, 4)

    candidates_by_position = [[] for _ in range(n)]

    # Ищем линейные комбинации исходных строк, которые подходят для каждой диагональной позиции.
    for coeffs in product(coefficient_range, repeat=n):
        if all(c == 0 for c in coeffs):
            continue

        new_row = [
            sum(
                coeffs[r] * A0[r][j]
                for r in range(n)
            )
            for j in range(n)
        ]

        new_rhs = sum(
            coeffs[r] * B0[r]
            for r in range(n)
        )

        for i in range(n):
            diagonal = abs(new_row[i])

            other_sum = sum(
                abs(new_row[j])
                for j in range(n)
                if j != i
            )

            if diagonal > other_sum + 1e-10:
                candidates_by_position[i].append({
                    "coeffs": list(coeffs),
                    "row": new_row[:],
                    "rhs": new_rhs,
                    "margin": diagonal - other_sum
                })

    # Сначала рассматриваем наиболее подходящие строки.
    for candidates in candidates_by_position:
        candidates.sort(
            key=lambda item: item["margin"],
            reverse=True
        )

    if any(not candidates for candidates in candidates_by_position):
        raise ValueError(
            "Не удалось найти линейную комбинацию "
            "для каждой строки. Попробуй расширить "
            "диапазон коэффициентов поиска."
        )

    selected = []
    transformation = None

    def search_rows(i):
        nonlocal transformation

        # Проверяем, что выбранные строки независимы.
        if i == n:
            C = [
                item["coeffs"][:]
                for item in selected
            ]

            if rank(C) == n:
                transformation = [
                    item.copy()
                    for item in selected
                ]
                return True

            return False

        for candidate in candidates_by_position[i]:
            selected.append(candidate)

            C_part = [
                item["coeffs"][:]
                for item in selected
            ]

            if rank(C_part) == len(C_part):
                if search_rows(i + 1):
                    return True

            selected.pop()

        return False

    if not search_rows(0) or transformation is None:
        raise ValueError(
            "Не удалось найти невырожденное "
            "преобразование со строгим диагональным "
            "преобладанием. Расширь диапазон поиска."
        )

    # Формируем новую матрицу и правую часть.
    A_work = [
        item["row"][:]
        for item in transformation
    ]

    B_work = [
        item["rhs"]
        for item in transformation
    ]

    transformations = []

    # Записываем выполненные линейные комбинации.
    for i, item in enumerate(transformation):
        coeffs = item["coeffs"]
        pieces = []

        for j, c in enumerate(coeffs):
            if c == 0:
                continue

            if c == 1:
                pieces.append(f"R{j + 1}")
            elif c == -1:
                pieces.append(f"-R{j + 1}")
            else:
                pieces.append(f"({c})*R{j + 1}")

        expression = " + ".join(pieces)
        expression = expression.replace("+ -", "- ")

        transformations.append(
            f"R{i + 1}' = {expression}"
        )

    # Отдельно проверяем строгое диагональное преобладание.
    for i in range(n):
        diagonal = abs(A_work[i][i])

        other_sum = sum(
            abs(A_work[i][j])
            for j in range(n)
            if j != i
        )

        if diagonal <= other_sum + 1e-10:
            raise ValueError(
                "Проверка диагонального преобладания "
                "не пройдена."
            )

        transformations.append(
            f"Строка {i + 1}: "
            f"|a{i + 1},{i + 1}| = {diagonal:.6g} "
            f"> сумма остальных модулей = {other_sum:.6g}"
        )

    return A_work, B_work, transformations