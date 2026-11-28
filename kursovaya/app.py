#!/usr/bin/env python3
"""Сетевая информационная система небольшого кинотеатра (только стандартная библиотека)."""
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse, parse_qs
import json
import sqlite3
import threading
from datetime import datetime, timedelta

ROOT = Path(__file__).resolve().parent
DB_PATH = ROOT / "cinema.sqlite3"
STATIC = ROOT / "static"
CONFIG_PATH = ROOT / "config.json"
WRITE_LOCK = threading.RLock()


def read_config():
    return json.loads(CONFIG_PATH.read_text(encoding="utf-8"))

SCHEMA = """
PRAGMA foreign_keys = ON;
CREATE TABLE IF NOT EXISTS films (
  film_code INTEGER PRIMARY KEY AUTOINCREMENT,
  title TEXT NOT NULL CHECK(length(trim(title)) BETWEEN 1 AND 200),
  genre TEXT NOT NULL,
  duration_min INTEGER NOT NULL CHECK(duration_min BETWEEN 1 AND 600),
  age_limit TEXT NOT NULL CHECK(age_limit IN ('0+','6+','12+','16+','18+')),
  description TEXT NOT NULL DEFAULT '', start_date TEXT, end_date TEXT
);
CREATE TABLE IF NOT EXISTS halls (
  hall_no INTEGER PRIMARY KEY AUTOINCREMENT,
  hall_name TEXT NOT NULL,
  row_count INTEGER NOT NULL CHECK(row_count > 0),
  capacity INTEGER NOT NULL CHECK(capacity > 0)
);
CREATE TABLE IF NOT EXISTS seats (
  hall_no INTEGER NOT NULL REFERENCES halls(hall_no),
  seat_row INTEGER NOT NULL CHECK(seat_row > 0),
  seat_no INTEGER NOT NULL CHECK(seat_no > 0),
  category TEXT NOT NULL DEFAULT 'Стандартное',
  PRIMARY KEY(hall_no, seat_row, seat_no)
);
CREATE TABLE IF NOT EXISTS employees (
  employee_no INTEGER PRIMARY KEY,
  surname TEXT NOT NULL,
  first_name TEXT NOT NULL,
  position TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS sessions (
  session_no INTEGER PRIMARY KEY AUTOINCREMENT,
  film_code INTEGER NOT NULL REFERENCES films(film_code),
  hall_no INTEGER NOT NULL REFERENCES halls(hall_no),
  starts_at TEXT NOT NULL,
  ticket_price REAL NOT NULL CHECK(ticket_price >= 0),
  state TEXT NOT NULL CHECK(state IN ('Запланирован','Отменен','Завершен')),
  UNIQUE(session_no, hall_no)
);
CREATE TABLE IF NOT EXISTS orders (
  order_no INTEGER PRIMARY KEY AUTOINCREMENT,
  created_at TEXT NOT NULL DEFAULT (datetime('now','localtime')),
  employee_no INTEGER NOT NULL REFERENCES employees(employee_no),
  payment_method TEXT NOT NULL CHECK(payment_method IN ('Наличные','Карта','Онлайн')),
  state TEXT NOT NULL CHECK(state IN ('Создан','Оплачен','Отменен'))
);
CREATE TABLE IF NOT EXISTS tickets (
  ticket_no INTEGER PRIMARY KEY AUTOINCREMENT,
  order_no INTEGER NOT NULL REFERENCES orders(order_no),
  session_no INTEGER NOT NULL REFERENCES sessions(session_no),
  hall_no INTEGER NOT NULL,
  seat_row INTEGER NOT NULL,
  seat_no INTEGER NOT NULL,
  sale_price REAL NOT NULL CHECK(sale_price >= 0),
  state TEXT NOT NULL CHECK(state IN ('Оплачен','Аннулирован')),
  UNIQUE(session_no, hall_no, seat_row, seat_no),
  FOREIGN KEY(hall_no, seat_row, seat_no) REFERENCES seats(hall_no, seat_row, seat_no),
  FOREIGN KEY(session_no, hall_no) REFERENCES sessions(session_no, hall_no)
);
CREATE INDEX IF NOT EXISTS ix_sessions_start ON sessions(starts_at);
CREATE INDEX IF NOT EXISTS ix_tickets_session ON tickets(session_no, state);
"""


def connect():
    db = sqlite3.connect(DB_PATH, timeout=15)
    db.row_factory = sqlite3.Row
    db.execute("PRAGMA foreign_keys=ON")
    return db


def initialize():
    with connect() as db:
        db.execute("PRAGMA journal_mode=WAL")
        db.executescript(SCHEMA)
        if db.execute("SELECT COUNT(*) FROM films").fetchone()[0] == 0:
            seed_demo_data(db)


def seed_demo_data(db):
    db.executemany("INSERT OR IGNORE INTO employees(employee_no,surname,first_name,position) VALUES(?,?,?,?)", [
        (1, "Иванов", "Иван", "Администратор"),
        (1001, "Петрова", "Анна", "Кассир"),
        (1002, "Сидоров", "Максим", "Кассир"),
        (1003, "Орлова", "Мария", "Кассир"),
    ])
    db.executemany("""INSERT INTO films(title,genre,duration_min,age_limit,description)
      VALUES(?,?,?,?,?)""", [
        ("Интерстеллар (2014)", "Фантастика, драма, приключения", 169, "18+", "Космическая экспедиция ищет новый дом для человечества."),
        ("Начало (2010)", "Фантастика, боевик, триллер", 148, "18+", "Команда внедряется в сновидения, чтобы изменить решение человека."),
        ("Тайна Коко (2017)", "Мультфильм, фэнтези, семейный", 105, "12+", "Юный музыкант попадает в мир мертвых и ищет историю своей семьи."),
        ("Король Лев (2019)", "Приключения, семейный, мюзикл", 118, "6+", "Львенок Симба проходит путь к своему месту в прайде."),
        ("Марсианин (2015)", "Фантастика, приключения, драма", 144, "18+", "Инженер пытается выжить на Марсе и дождаться помощи."),
        ("Пираты Карибского моря: Проклятие Черной жемчужины (2003)", "Приключения, фэнтези, боевик", 143, "12+", "Капитан Джек Воробей отправляется на поиски корабля и сокровищ."),
        ("Форрест Гамп (1994)", "Драма, комедия", 142, "18+", "История жизни Форреста Гампа на фоне событий американской истории."),
        ("История игрушек (1995)", "Мультфильм, комедия, семейный", 81, "0+", "Игрушки оживают, когда их не видят люди."),
        ("Гарри Поттер и философский камень (2001)", "Фэнтези, приключения, семейный", 152, "12+", "Гарри поступает в Хогвартс и знакомится с миром магии."),
    ])
    hall_specs = [("Зал 1", 5, 8), ("Зал 2", 4, 6)]
    for name, row_count, seats_per_row in hall_specs:
        cur = db.execute("INSERT INTO halls(hall_name,row_count,capacity) VALUES(?,?,?)",
                         (name, row_count, row_count * seats_per_row))
        hall_no = cur.lastrowid
        db.executemany("INSERT INTO seats(hall_no,seat_row,seat_no) VALUES(?,?,?)",
                       [(hall_no, row, seat) for row in range(1, row_count + 1)
                        for seat in range(1, seats_per_row + 1)])
    base = (datetime.now() + timedelta(hours=2)).replace(minute=0, second=0, microsecond=0)
    showings = [
        (1, 1, base, 500.0),
        (2, 1, base + timedelta(hours=3), 550.0),
        (3, 2, base + timedelta(hours=1), 400.0),
        (1, 2, base + timedelta(days=1, hours=3), 500.0),
        (4, 1, base.replace(hour=9) + timedelta(days=1), 470.0),
        (5, 1, base.replace(hour=12) + timedelta(days=1), 450.0),
        (6, 1, base.replace(hour=15) + timedelta(days=1), 520.0),
        (7, 2, base.replace(hour=9) + timedelta(days=2), 430.0),
        (8, 2, base.replace(hour=12) + timedelta(days=2), 490.0),
        (9, 2, base.replace(hour=15) + timedelta(days=2), 420.0),
    ]
    db.executemany("""INSERT INTO sessions(film_code,hall_no,starts_at,ticket_price,state)
      VALUES(?,?,?,?, 'Запланирован')""",
      [(film, hall, starts.strftime("%Y-%m-%d %H:%M:%S"), price)
       for film, hall, starts, price in showings])
    # Несколько демонстрационных продаж показывают занятые места и заполняют отчеты.
    order = db.execute("INSERT INTO orders(employee_no,payment_method,state) VALUES(1001,'Карта','Оплачен')")
    order_no = order.lastrowid
    db.executemany("""INSERT INTO tickets(order_no,session_no,hall_no,seat_row,seat_no,sale_price,state)
      VALUES(?,?,1,1,?,500,'Оплачен')""", [(order_no, 1, seat) for seat in (1, 2)])
    order = db.execute("INSERT INTO orders(employee_no,payment_method,state) VALUES(1002,'Наличные','Оплачен')")
    db.execute("""INSERT INTO tickets(order_no,session_no,hall_no,seat_row,seat_no,sale_price,state)
      VALUES(?,1,1,2,1,500,'Оплачен')""", (order.lastrowid,))
    demo_orders = [
        (1003, "Карта", 2, 1, 1, [1, 2]),
        (1001, "Наличные", 3, 2, 1, [1, 2]),
        (1003, "Онлайн", 5, 1, 1, [1, 2, 3]),
        (1002, "Карта", 8, 2, 1, [1]),
    ]
    for employee_no, payment, session_no, hall_no, seat_row, seat_numbers in demo_orders:
        session = db.execute("SELECT ticket_price FROM sessions WHERE session_no=?", (session_no,)).fetchone()
        order = db.execute("INSERT INTO orders(employee_no,payment_method,state) VALUES(?,?, 'Оплачен')",
                           (employee_no, payment))
        db.executemany("""INSERT INTO tickets(order_no,session_no,hall_no,seat_row,seat_no,sale_price,state)
          VALUES(?,?,?,?,?,?,'Оплачен')""",
          [(order.lastrowid, session_no, hall_no, seat_row, seat, session[0]) for seat in seat_numbers])


def reset_demo_data():
    with WRITE_LOCK, connect() as db:
        db.execute("BEGIN IMMEDIATE")
        for table in ("tickets", "orders", "sessions", "seats", "halls", "films", "employees"):
            db.execute(f"DELETE FROM {table}")
        db.execute("DELETE FROM sqlite_sequence")
        seed_demo_data(db)


def json_bytes(value):
    return json.dumps(value, ensure_ascii=False, default=str).encode("utf-8")


class Handler(BaseHTTPRequestHandler):
    server_version = "CinemaIS/1.0"

    def send_data(self, status, body, content_type="application/json; charset=utf-8"):
        data = body if isinstance(body, bytes) else json_bytes(body)
        self.send_response(status)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(data)))
        self.send_header("X-Content-Type-Options", "nosniff")
        self.end_headers()
        self.wfile.write(data)

    def read_json(self):
        length = int(self.headers.get("Content-Length", "0"))
        if length > 1_000_000:
            raise ValueError("Слишком большой запрос")
        return json.loads(self.rfile.read(length) or b"{}")

    def do_GET(self):
        path = urlparse(self.path).path
        if path == "/" or path == "/index.html":
            self.serve_static("index.html", "text/html; charset=utf-8"); return
        if path == "/styles.css":
            self.serve_static("styles.css", "text/css; charset=utf-8"); return
        if path == "/app.js":
            self.serve_static("app.js", "text/javascript; charset=utf-8"); return
        try:
            with connect() as db:
                if path == "/api/films":
                    rows = db.execute("SELECT * FROM films ORDER BY title").fetchall()
                elif path == "/api/config":
                    self.send_data(200, read_config()); return
                elif path == "/api/halls":
                    rows = db.execute("SELECT * FROM halls ORDER BY hall_no").fetchall()
                elif path == "/api/employees":
                    rows = db.execute("SELECT * FROM employees ORDER BY surname").fetchall()
                elif path == "/api/search":
                    q = parse_qs(urlparse(self.path).query)
                    term = int(q.get("term", ["0"])[0])
                    order = db.execute("""SELECT o.order_no,o.created_at,o.payment_method,o.state order_state,
                      e.surname,e.first_name FROM orders o JOIN employees e USING(employee_no)
                      WHERE o.order_no=? OR EXISTS(SELECT 1 FROM tickets t WHERE t.order_no=o.order_no AND t.ticket_no=?)
                      ORDER BY CASE WHEN o.order_no=? THEN 0 ELSE 1 END, o.order_no DESC LIMIT 1""", (term, term, term)).fetchone()
                    if not order:
                        self.send_data(404, {"error": "Заказ или билет не найден"}); return
                    tickets = db.execute("""SELECT t.ticket_no,t.seat_row,t.seat_no,t.sale_price,t.state ticket_state,
                      f.title,s.starts_at,h.hall_name FROM tickets t JOIN sessions s USING(session_no)
                      JOIN films f USING(film_code) JOIN halls h USING(hall_no) WHERE t.order_no=?
                      ORDER BY t.ticket_no""", (order["order_no"],)).fetchall()
                    self.send_data(200, {**dict(order), "tickets": [dict(t) for t in tickets]}); return
                elif path == "/api/sessions":
                    rows = db.execute("""SELECT s.*, f.title, f.age_limit, f.duration_min, h.hall_name, h.capacity,
                      (SELECT COUNT(*) FROM tickets t WHERE t.session_no=s.session_no AND t.state='Оплачен') sold
                      FROM sessions s JOIN films f USING(film_code) JOIN halls h USING(hall_no)
                      WHERE s.state='Запланирован' ORDER BY s.starts_at""").fetchall()
                elif path.startswith("/api/seats/"):
                    session_no = int(path.rsplit("/", 1)[1])
                    rows = db.execute("""SELECT st.seat_row, st.seat_no, st.category,
                      CASE WHEN t.ticket_no IS NULL THEN 0 ELSE 1 END sold
                      FROM sessions s JOIN seats st ON st.hall_no=s.hall_no
                      LEFT JOIN tickets t ON t.session_no=s.session_no AND t.hall_no=st.hall_no
                        AND t.seat_row=st.seat_row AND t.seat_no=st.seat_no AND t.state='Оплачен'
                      WHERE s.session_no=? ORDER BY st.seat_row,st.seat_no""", (session_no,)).fetchall()
                elif path == "/api/reports/sales":
                    q = parse_qs(urlparse(self.path).query)
                    start, end = (q.get("start", [""])[0], q.get("end", [""])[0])
                    rows = db.execute("""SELECT o.created_at, o.order_no, t.ticket_no, f.title,
                      s.starts_at, t.hall_no, t.seat_row, t.seat_no, o.payment_method,
                      t.sale_price, e.surname
                      FROM tickets t JOIN orders o USING(order_no) JOIN employees e USING(employee_no)
                      JOIN sessions s USING(session_no) JOIN films f USING(film_code)
                      WHERE t.state='Оплачен' AND date(o.created_at) BETWEEN COALESCE(NULLIF(?,''),'0001-01-01')
                        AND COALESCE(NULLIF(?,''),'9999-12-31') ORDER BY o.created_at DESC""", (start, end)).fetchall()
                elif path == "/api/reports/sessions":
                    q = parse_qs(urlparse(self.path).query)
                    start, end = (q.get("start", [""])[0], q.get("end", [""])[0])
                    rows = db.execute("""SELECT s.starts_at,f.title,h.hall_name,h.capacity,
                      COUNT(CASE WHEN t.state='Оплачен' THEN 1 END) sold,
                      ROUND(100.0*COUNT(CASE WHEN t.state='Оплачен' THEN 1 END)/h.capacity,1) occupancy,
                      COALESCE(SUM(CASE WHEN t.state='Оплачен' THEN t.sale_price ELSE 0 END),0) revenue
                      FROM sessions s JOIN films f USING(film_code) JOIN halls h USING(hall_no)
                      LEFT JOIN tickets t USING(session_no)
                      WHERE date(s.starts_at) BETWEEN COALESCE(NULLIF(?,''),'0001-01-01')
                        AND COALESCE(NULLIF(?,''),'9999-12-31')
                      GROUP BY s.session_no ORDER BY s.starts_at""", (start,end)).fetchall()
                else:
                    self.send_data(404, {"error": "Маршрут не найден"}); return
            self.send_data(200, [dict(r) for r in rows])
        except (ValueError, sqlite3.Error) as exc:
            self.send_data(400, {"error": str(exc)})

    def serve_static(self, name, content_type):
        target = STATIC / name
        if not target.is_file():
            self.send_data(404, b"Not found", "text/plain; charset=utf-8"); return
        self.send_data(200, target.read_bytes(), content_type)

    def do_POST(self):
        path = urlparse(self.path).path
        try:
            data = self.read_json()
            if path == "/api/reset":
                reset_demo_data()
                self.send_data(200, {"ok": True, "message": "Демо-данные восстановлены"})
                return
            with WRITE_LOCK, connect() as db:
                if path == "/api/films":
                    cur = db.execute("""INSERT INTO films(title,genre,duration_min,age_limit,description,start_date,end_date)
                      VALUES(?,?,?,?,?,?,?)""", (data["title"].strip(),data.get("genre","Драма"),int(data["duration_min"]),
                      data.get("age_limit","16+"),data.get("description",""),data.get("start_date") or None,data.get("end_date") or None))
                    result = {"film_code": cur.lastrowid}
                elif path == "/api/halls":
                    rows, cols = int(data["rows"]), int(data["seats_per_row"])
                    if rows < 1 or cols < 1 or rows * cols > 1000: raise ValueError("Недопустимое количество мест")
                    cur = db.execute("INSERT INTO halls(hall_name,row_count,capacity) VALUES(?,?,?)", (data["hall_name"],rows,rows*cols))
                    hall_no = cur.lastrowid
                    db.executemany("INSERT INTO seats(hall_no,seat_row,seat_no) VALUES(?,?,?)",
                                   [(hall_no,r,c) for r in range(1,rows+1) for c in range(1,cols+1)])
                    result = {"hall_no": hall_no, "capacity": rows*cols}
                elif path == "/api/employees":
                    db.execute("INSERT INTO employees(employee_no,surname,first_name,position) VALUES(?,?,?,?)",
                               (int(data["employee_no"]),data["surname"],data["first_name"],data["position"]))
                    result = {"employee_no": int(data["employee_no"])}
                elif path == "/api/sessions":
                    film = db.execute("SELECT duration_min FROM films WHERE film_code=?", (int(data["film_code"]),)).fetchone()
                    if not film: raise ValueError("Фильм не найден")
                    hall_no, starts_at = int(data["hall_no"]), data["starts_at"]
                    # Не допускаем пересечение фактических интервалов показов.
                    conflict = db.execute("""SELECT 1 FROM sessions s JOIN films f USING(film_code)
                      WHERE s.hall_no=? AND s.state='Запланирован'
                      AND datetime(?) < datetime(s.starts_at, '+' || f.duration_min || ' minutes')
                      AND datetime(s.starts_at) < datetime(?, '+' || ? || ' minutes')""",
                      (hall_no,starts_at,starts_at,int(film[0]))).fetchone()
                    if conflict: raise ValueError("В этом зале уже назначен пересекающийся сеанс")
                    cur = db.execute("INSERT INTO sessions(film_code,hall_no,starts_at,ticket_price,state) VALUES(?,?,?,?,?)",
                      (int(data["film_code"]),hall_no,starts_at,float(data["ticket_price"]),"Запланирован"))
                    result = {"session_no": cur.lastrowid}
                elif path == "/api/sales":
                    session_no, employee_no = int(data["session_no"]), int(data["employee_no"])
                    seats = data["seats"]
                    if not seats: raise ValueError("Выберите хотя бы одно место")
                    session = db.execute("SELECT hall_no,ticket_price,state FROM sessions WHERE session_no=?",(session_no,)).fetchone()
                    if not session or session["state"] != "Запланирован": raise ValueError("Сеанс недоступен")
                    db.execute("BEGIN IMMEDIATE")
                    cur = db.execute("INSERT INTO orders(employee_no,payment_method,state) VALUES(?,?,?)",
                                     (employee_no,data["payment_method"],"Оплачен"))
                    order_no = cur.lastrowid
                    for seat in seats:
                        db.execute("INSERT INTO tickets(order_no,session_no,hall_no,seat_row,seat_no,sale_price,state) VALUES(?,?,?,?,?,?,?)",
                          (order_no,session_no,int(session["hall_no"]),int(seat["row"]),int(seat["number"]),float(session["ticket_price"]),"Оплачен"))
                    result = {"order_no": order_no, "ticket_count": len(seats), "total": round(float(session["ticket_price"])*len(seats),2)}
                else:
                    self.send_data(404, {"error": "Маршрут не найден"}); return
            self.send_data(201, result)
        except (KeyError, ValueError, TypeError, sqlite3.Error) as exc:
            self.send_data(400, {"error": str(exc) or "Некорректные данные"})

    def log_message(self, fmt, *args):
        print("%s - %s" % (self.address_string(), fmt % args))


if __name__ == "__main__":
    initialize()
    host, port = "0.0.0.0", 8000
    print(f"{read_config()['cinema_name']} запущен: http://localhost:{port} (сеть: http://<IP-сервера>:{port})")
    ThreadingHTTPServer((host, port), Handler).serve_forever()
