import { useState } from 'react'
import './App.css'
import RegisterUserPage from './pages/RegisterUserPage'
import UserManagementPage from './pages/UserManagementPage'
import QueuePage from './pages/QueuePage'
import ReportsPage from './pages/ReportsPage'

const PAGES = [
  { key: 'registro', label: 'Registro', component: <RegisterUserPage /> },
  { key: 'usuarios', label: 'Gestión de Usuarios', component: <UserManagementPage /> },
  { key: 'cola', label: 'Cola de Prioridad', component: <QueuePage /> },
  { key: 'reportes', label: 'Reportes', component: <ReportsPage /> },
]

function App() {
  const [page, setPage] = useState('registro')

  const current = PAGES.find(p => p.key === page)

  return (
    <div>
      <nav>
        <ul style={{ display: 'flex', gap: '1rem', justifyContent: 'center', listStyle: 'none', padding: 0 }}>
          {PAGES.map(p => (
            <li key={p.key}>
              <button
                style={{ background: page === p.key ? '#e0e0e0' : 'transparent', border: 'none', cursor: 'pointer', fontWeight: 'bold' }}
                onClick={() => setPage(p.key)}
              >
                {p.label}
              </button>
            </li>
          ))}
        </ul>
      </nav>
      <hr />
      <div>
        {current ? current.component : (
          <>
            <h2>Bienvenido al Sistema de Control de Accesos</h2>
            <p>Selecciona una opción en el menú para comenzar.</p>
          </>
        )}
      </div>
    </div>
  )
}

export default App
