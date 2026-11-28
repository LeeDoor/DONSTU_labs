const $ = (selector) => document.querySelector(selector);
const selectedSeats = new Map();
let currentRole = '';
let sessions = [], films = [], halls = [], employees = [];

const functionsByRole = {
  cashier: [
    {id:'sale', title:'Афиша и продажа билетов', description:'Выбрать сеанс и свободные места, оформить продажу', icon:'▣'},
    {id:'search', title:'Найти заказ или билет', description:'Найти оформленный заказ по его номеру или номеру билета', icon:'⌕'},
    {id:'shift-report', title:'Отчёт о продажах за смену', description:'Посмотреть кассовые операции за дату или период', icon:'▤'}
  ],
  admin: [
    {id:'films', title:'Фильмы', description:'Просмотреть каталог и добавить фильм', icon:'▰'},
    {id:'halls', title:'Залы и места', description:'Просмотреть залы или создать зал с местами', icon:'▥'},
    {id:'schedule', title:'Расписание сеансов', description:'Сформировать расписание и проверить занятость залов', icon:'◷'},
    {id:'session-report', title:'Посещаемость и выручка', description:'Сводный отчёт по сеансам за выбранный период', icon:'▤'},
    {id:'employees', title:'Сотрудники', description:'Просмотреть список и добавить сотрудника', icon:'♙'}
  ]
};

async function api(url, options = {}) {
  const response = await fetch(url, {headers:{'Content-Type':'application/json'}, ...options});
  const data = await response.json().catch(()=>({}));
  if (!response.ok) throw new Error(data.error || 'Не удалось выполнить операцию');
  return data;
}
function escapeHtml(value) {
  return String(value ?? '').replace(/[&<>"']/g, ch=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[ch]));
}
function toast(message) {
  const node=$('#toast'); node.textContent=message; node.classList.add('show');
  setTimeout(()=>node.classList.remove('show'),3500);
}
function money(value) { return new Intl.NumberFormat('ru-RU',{style:'currency',currency:'RUB',maximumFractionDigits:2}).format(value||0); }
function dateTime(value) { return new Date(String(value).replace(' ','T')).toLocaleString('ru-RU',{dateStyle:'short',timeStyle:'short'}); }
function fillSelect(selector, rows, value, label, emptyText) {
  const select=$(selector);
  select.innerHTML=rows.length?rows.map(r=>`<option value="${escapeHtml(r[value])}">${escapeHtml(label(r))}</option>`).join(''):`<option value="">${emptyText}</option>`;
}
function selectRole(role) {
  currentRole=role;
  const isCashier=role==='cashier';
  $('#current-role').textContent=isCashier?'Роль: кассир':'Роль: администратор';
  $('#role-view').classList.add('hidden'); $('#function-view').classList.add('hidden');
  $('#menu-view').classList.remove('hidden'); $('#menu-button').classList.add('hidden'); $('#exit-button').classList.remove('hidden');
  $('#menu-eyebrow').textContent=isCashier?'РАБОЧЕЕ МЕСТО КАССИРА':'РАБОЧЕЕ МЕСТО АДМИНИСТРАТОРА';
  $('#menu-title').textContent=isCashier?'Функции кассира':'Функции администратора';
  $('#menu-description').textContent=isCashier?'Выберите действие для обслуживания посетителя и работы с кассой.':'Выберите действие для управления кинотеатром и контроля показателей.';
  $('#function-grid').innerHTML=functionsByRole[role].map(item=>`<button class="function-card" data-function="${item.id}"><span class="function-icon">${item.icon}</span><strong>${item.title}</strong><small>${item.description}</small><span class="card-arrow">Открыть →</span></button>`).join('');
  $('#function-grid').querySelectorAll('[data-function]').forEach(button=>button.addEventListener('click',()=>openFunction(button.dataset.function)));
  loadBase().catch(e=>toast(e.message));
}
function showMenu() {
  $('#function-view').classList.add('hidden'); $('#menu-view').classList.remove('hidden');
  $('#menu-button').classList.add('hidden'); $('#exit-button').classList.remove('hidden');
}
function showRolePicker() {
  currentRole=''; $('#current-role').textContent='';
  $('#menu-view').classList.add('hidden'); $('#function-view').classList.add('hidden'); $('#role-view').classList.remove('hidden');
  $('#exit-button').classList.add('hidden'); $('#menu-button').classList.add('hidden');
}
function openFunction(id) {
  const item=functionsByRole[currentRole].find(fn=>fn.id===id);
  if(!item) return;
  $('#menu-view').classList.add('hidden'); $('#function-view').classList.remove('hidden');
  $('#menu-button').classList.remove('hidden'); $('#exit-button').classList.add('hidden');
  $('#function-title').textContent=item.title;
  $('#function-eyebrow').textContent=currentRole==='cashier'?'ФУНКЦИИ КАССИРА':'ФУНКЦИИ АДМИНИСТРАТОРА';
  document.querySelectorAll('.function-content').forEach(section=>section.classList.add('hidden'));
  $(`#function-${id}`).classList.remove('hidden');
  if(id==='sale') loadSeatMap($('#session-select').value).catch(e=>toast(e.message));
  if(id==='search') $('#search-term').focus();
  if(id==='shift-report') loadSalesReport();
  if(id==='films') renderFilms();
  if(id==='halls') renderHalls();
  if(id==='schedule') renderSchedule();
  if(id==='session-report') loadSessionReport();
  if(id==='employees') renderEmployees();
}
async function loadBase() {
  let config;
  [films,halls,employees,sessions,config]=await Promise.all([
    api('/api/films'),api('/api/halls'),api('/api/employees'),api('/api/sessions'),api('/api/config')
  ]);
  $('.brand strong').textContent=config.cinema_name;
  document.title=`${config.cinema_name} — Информационная система`;
  fillSelect('#film-select',films,'film_code',f=>f.title,'Сначала добавьте фильм');
  fillSelect('#hall-select',halls,'hall_no',h=>`${h.hall_name} · ${h.capacity} мест`,'Сначала добавьте зал');
  fillSelect('#employee-select',employees,'employee_no',e=>`${e.surname} ${e.first_name}`,'Добавьте сотрудника');
  fillSelect('#session-select',sessions,'session_no',s=>`${dateTime(s.starts_at)} · ${s.title} · ${s.hall_name}`,'Нет запланированных сеансов');
  if(sessions.length) await loadSeatMap($('#session-select').value);
  else { $('#seat-map').innerHTML='<p class="muted">Нет доступных сеансов.</p>'; $('#session-card').textContent='Добавьте фильм, зал и сеанс.'; }
  renderFilms(); renderHalls(); renderSchedule(); renderEmployees();
}
function renderTable(target, rows, columns, emptyText) {
  const node=$(target);
  node.innerHTML=rows.length?rows.map(row=>`<tr>${columns.map(col=>`<td>${col(row)}</td>`).join('')}</tr>`).join(''):`<tr><td colspan="${columns.length}" class="muted">${emptyText}</td></tr>`;
}
function renderFilms(){renderTable('#films-table',films,[f=>f.film_code,f=>escapeHtml(f.title),f=>escapeHtml(f.genre),f=>`${f.duration_min} мин`,f=>escapeHtml(f.age_limit)],'Фильмов пока нет.');}
function renderHalls(){renderTable('#halls-table',halls,[h=>h.hall_no,h=>escapeHtml(h.hall_name),h=>h.row_count,h=>h.capacity],'Залов пока нет.');}
function renderEmployees(){renderTable('#employees-table',employees,[e=>e.employee_no,e=>escapeHtml(e.surname),e=>escapeHtml(e.first_name),e=>escapeHtml(e.position)],'Сотрудников пока нет.');}
function renderSchedule(){renderTable('#schedule-table',sessions,[s=>dateTime(s.starts_at),s=>escapeHtml(s.title),s=>escapeHtml(s.hall_name),s=>money(s.ticket_price),s=>s.sold,s=>Math.max(0,s.capacity-s.sold)],'Запланированных сеансов пока нет.');}
async function loadSeatMap(id) {
  selectedSeats.clear(); $('#sell').disabled=true; updateSelection();
  const session=sessions.find(s=>String(s.session_no)===String(id));
  if(!session){$('#seat-map').innerHTML='<p class="muted">Выберите доступный сеанс.</p>';return;}
  $('#session-card').innerHTML=`<strong>${escapeHtml(session.title)}</strong><br>${dateTime(session.starts_at)} · ${escapeHtml(session.hall_name)}<br>${session.duration_min} мин · ${escapeHtml(session.age_limit)} · ${money(session.ticket_price)}<br>Свободно: ${Math.max(0,session.capacity-session.sold)} из ${session.capacity}`;
  const seats=await api(`/api/seats/${id}`), rows=new Map();
  seats.forEach(seat=>{if(!rows.has(seat.seat_row))rows.set(seat.seat_row,[]);rows.get(seat.seat_row).push(seat);});
  $('#seat-map').innerHTML=[...rows.entries()].map(([row,items])=>`<div class="seat-row" aria-label="Ряд ${row}">${items.map(seat=>`<button class="seat ${seat.sold?'sold':'free'}" title="Ряд ${row}, место ${seat.seat_no}${seat.sold?' · продано':''}" ${seat.sold?'disabled':''} data-row="${row}" data-number="${seat.seat_no}"></button>`).join('')}</div>`).join('');
  $('#seat-map').querySelectorAll('.seat.free').forEach(button=>button.addEventListener('click',()=>{
    const key=`${button.dataset.row}:${button.dataset.number}`;
    if(selectedSeats.has(key)){selectedSeats.delete(key);button.classList.remove('chosen');}
    else{selectedSeats.set(key,{row:Number(button.dataset.row),number:Number(button.dataset.number)});button.classList.add('chosen');}
    updateSelection();
  }));
}
function updateSelection(){
  const count=selectedSeats.size, session=sessions.find(s=>String(s.session_no)===$('#session-select').value);
  $('#selection-summary').textContent=count&&session?`${count} ${count===1?'билет':'билета'} · ${money(count*session.ticket_price)}`:'Билеты не выбраны';
  $('#sell').disabled=count===0;
}
async function submitForm(form,url,transform=x=>x) {
  const body=transform(Object.fromEntries(new FormData(form).entries()));
  try{await api(url,{method:'POST',body:JSON.stringify(body)});form.reset();await loadBase();toast('Данные сохранены');}
  catch(e){toast(e.message);}
}
async function loadSalesReport(){
  const params=new URLSearchParams({start:$('#date-start').value,end:$('#date-end').value});
  try{
    const rows=await api(`/api/reports/sales?${params}`);
    renderTable('#sales-report',rows,[r=>dateTime(r.created_at),r=>r.order_no,r=>r.ticket_no,r=>escapeHtml(r.title),r=>dateTime(r.starts_at),r=>`Зал ${r.hall_no}, ${r.seat_row} ряд, ${r.seat_no} место`,r=>escapeHtml(r.payment_method),r=>money(r.sale_price),r=>escapeHtml(r.surname)],'За выбранный период продаж нет.');
    const amount=rows.reduce((sum,row)=>sum+Number(row.sale_price),0);
    $('#sales-total').textContent=`Продано билетов: ${rows.length} · Общая сумма: ${money(amount)}`;
  }catch(e){toast(e.message);}
}
async function loadSessionReport(){
  const params=new URLSearchParams({start:$('#session-date-start').value,end:$('#session-date-end').value});
  try{const rows=await api(`/api/reports/sessions?${params}`);renderTable('#sessions-report',rows,[r=>dateTime(r.starts_at),r=>escapeHtml(r.title),r=>escapeHtml(r.hall_name),r=>r.capacity,r=>r.sold,r=>`${r.occupancy}%`,r=>money(r.revenue)],'Сеансов за выбранный период нет.');}
  catch(e){toast(e.message);}
}
async function searchOrderOrTicket(){
  const result=$('#search-result');result.innerHTML='<p class="muted">Поиск…</p>';
  try{
    const data=await api(`/api/search?term=${encodeURIComponent($('#search-term').value)}`);
    if(!data.tickets.length){result.innerHTML='<p class="muted">Заказ или билет не найден.</p>';return;}
    result.innerHTML=`<h3>Заказ №${data.order_no}</h3><p>Оформлен: ${dateTime(data.created_at)} · Кассир: ${escapeHtml(data.surname)} ${escapeHtml(data.first_name)} · Оплата: ${escapeHtml(data.payment_method)} · Состояние: ${escapeHtml(data.order_state)}</p><div class="table-wrap"><table><thead><tr><th>Билет</th><th>Фильм и сеанс</th><th>Зал и место</th><th>Цена</th><th>Состояние</th></tr></thead><tbody>${data.tickets.map(t=>`<tr><td>${t.ticket_no}</td><td>${escapeHtml(t.title)} · ${dateTime(t.starts_at)}</td><td>${escapeHtml(t.hall_name)}, ряд ${t.seat_row}, место ${t.seat_no}</td><td>${money(t.sale_price)}</td><td>${escapeHtml(t.ticket_state)}</td></tr>`).join('')}</tbody></table></div>`;
  }catch(e){result.innerHTML=`<p class="muted">${escapeHtml(e.message)}</p>`;}
}

document.querySelectorAll('[data-role]').forEach(button=>button.addEventListener('click',()=>selectRole(button.dataset.role)));
$('#home-button').addEventListener('click',showRolePicker);
$('#exit-button').addEventListener('click',showRolePicker);
$('#menu-button').addEventListener('click',showMenu);
$('#back-to-menu').addEventListener('click',showMenu);
$('#reset-data').addEventListener('click',async()=>{
  const accepted=window.confirm('Удалить текущие данные и восстановить демонстрационные фильмы, залы, сеансы, сотрудников и продажи?');
  if(!accepted)return;
  try{
    await api('/api/reset',{method:'POST',body:JSON.stringify({})});
    showRolePicker();
    await loadBase();
    toast('Демо-база восстановлена');
  }catch(e){toast(e.message);}
});
$('#session-select').addEventListener('change',e=>loadSeatMap(e.target.value).catch(err=>toast(err.message)));
$('#refresh').addEventListener('click',()=>loadBase().catch(e=>toast(e.message)));
$('#refresh-schedule').addEventListener('click',()=>loadBase().catch(e=>toast(e.message)));
$('#sell').addEventListener('click',async()=>{
  const session_no=Number($('#session-select').value);
  try{
    const receipt=await api('/api/sales',{method:'POST',body:JSON.stringify({session_no,employee_no:Number($('#employee-select').value),payment_method:$('#payment-select').value,seats:[...selectedSeats.values()]})});
    toast(`Заказ №${receipt.order_no}: билетов ${receipt.ticket_count}, сумма ${money(receipt.total)}`);
    await loadBase();
  }catch(e){toast(e.message);await loadBase();}
});
$('#film-form').addEventListener('submit',e=>{e.preventDefault();submitForm(e.target,'/api/films',d=>({...d,duration_min:Number(d.duration_min)}));});
$('#hall-form').addEventListener('submit',e=>{e.preventDefault();submitForm(e.target,'/api/halls',d=>({...d,rows:Number(d.rows),seats_per_row:Number(d.seats_per_row)}));});
$('#employee-form').addEventListener('submit',e=>{e.preventDefault();submitForm(e.target,'/api/employees',d=>({...d,employee_no:Number(d.employee_no)}));});
$('#session-form').addEventListener('submit',e=>{e.preventDefault();submitForm(e.target,'/api/sessions',d=>({...d,film_code:Number(d.film_code),hall_no:Number(d.hall_no),ticket_price:Number(d.ticket_price),starts_at:d.starts_at.replace('T',' ')+':00'}));});
$('#search-form').addEventListener('submit',e=>{e.preventDefault();searchOrderOrTicket();});
$('#load-sales').addEventListener('click',loadSalesReport);
$('#load-sessions-report').addEventListener('click',loadSessionReport);
$('#today').textContent=new Date().toLocaleDateString('ru-RU',{weekday:'long',day:'numeric',month:'long',year:'numeric'});
api('/api/config').then(config=>{$('.brand strong').textContent=config.cinema_name;document.title=`${config.cinema_name} — Информационная система`;}).catch(e=>toast(e.message));
