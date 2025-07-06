import { useEffect, useState } from 'react'
import { getUsuarios, encolarUsuario, actualizarPerfil, actualizarPrioridadCola, buscarUsuario } from '../api/cliente'
import './UserList.css'

const perfiles = [
    { value: "vip", label: "VIP" },
    { value: "personal-medico", label: "Personal Medico" },
    { value: "seguridad", label: "Seguridad" },
    { value: "discapacitados", label: "Discapacitados" },
    { value: "publico-general", label: "Publico General" }
]

function EditProfileModal({ usuario, onClose, onUpdated, onQueueRefresh }) {
    const [perfil, setPerfil] = useState(usuario.perfil)
    const [msg, setMsg] = useState(null)
    const [loading, setLoading] = useState(false)

    const handleSave = async (e) => {
        e.preventDefault()
        setMsg(null)
        setLoading(true)
        try {
            // 1. Actualizar perfil en hash
            const res = await actualizarPerfil(usuario.dni, perfil)
            if (!res.ok) {
                setMsg('Error al actualizar perfil')
                setLoading(false)
                return
            }
            // 2. Ver si está en cola y actualizar prioridad si corresponde
            const userInfo = await buscarUsuario(usuario.dni)
            if (userInfo && userInfo.valid && userInfo.enCola) {
                // Llamar a PUT /cola/update para reflejar el cambio en el heap
                const res2 = await actualizarPrioridadCola(usuario.dni, perfil)
                if (!res2.ok) {
                    setMsg('Error al actualizar prioridad en cola')
                    setLoading(false)
                    return
                }
                if (onQueueRefresh) onQueueRefresh()
            }
            setMsg('Perfil actualizado')
            onUpdated()
            onClose()
        } catch {
            setMsg('Error al actualizar')
        }
        setLoading(false)
    }

    return (
        <div className="modal-bg">
            <div className="modal">
                <h4>Editar perfil</h4>
                <form onSubmit={handleSave}>
                    <select value={perfil} onChange={e => setPerfil(e.target.value)}>
                        {perfiles.map(p => <option key={p.value} value={p.value}>{p.label}</option>)}
                    </select>
                    <div style={{ marginTop: '1rem' }}>
                        <button type="submit" disabled={loading}>Guardar</button>
                        <button type="button" onClick={onClose} style={{ marginLeft: '1rem' }}>Cancelar</button>
                    </div>
                </form>
                {msg && <div className="edit-profile-msg">{msg}</div>}
            </div>
        </div>
    )
}

function mostrarPerfil(perfil) {
    if (!perfil) return '-'
    return perfil
        .split(/[\s\-]+/)
        .map(p => p.charAt(0).toUpperCase() + p.slice(1))
        .join(' ')
}

export default function UserList({ onQueueRefresh, editable = true }) {
    const [usuarios, setUsuarios] = useState([])
    const [loading, setLoading] = useState(true)
    const [editUser, setEditUser] = useState(null)
    const [msg, setMsg] = useState(null)

    const fetchUsuarios = () => {
        setLoading(true)
        getUsuarios().then(data => {
            setUsuarios(data)
            setLoading(false)
        })
    }

    useEffect(() => {
        fetchUsuarios()
    }, [])

    const handleEncolar = async (u) => {
        setMsg(null)
        // Siempre usar timestamp actual en segundos
        const ts = Math.floor(Date.now() / 1000)
        try {
            const res = await encolarUsuario(u.dni, ts)
            if (res.status === 201 || res.ok) {
                setMsg('Usuario encolado')
                fetchUsuarios()
            } else if (res.status === 409) {
                setMsg('Usuario ya fue procesado')
            } else {
                setMsg('Error al encolar usuario')
            }
        } catch {
            setMsg('Error al encolar usuario')
        }
    }

    return (
        <div className="user-list">
            <h3>Usuarios no atendidos</h3>
            {msg && (
                <div
                    className={
                        "user-list-msg " +
                        (msg === 'Usuario encolado' || msg === 'Perfil actualizado'
                            ? "success"
                            : "error")
                    }
                >
                    {msg}
                </div>
            )}
            {loading ? <div>Cargando...</div> : (
                <table>
                    <thead>
                        <tr>
                            <th>DNI</th>
                            <th>Perfil</th>
                            <th>Acciones</th>
                        </tr>
                    </thead>
                    <tbody>
                        {usuarios.map(u => (
                            <tr key={u.dni}>
                                <td>{u.dni}</td>
                                <td>{mostrarPerfil(u.perfil)}</td>
                                <td>
                                    <button onClick={() => handleEncolar(u)}>Encolar</button>
                                    {editable && (
                                        <button onClick={() => setEditUser(u)}>Editar perfil</button>
                                    )}
                                </td>
                            </tr>
                        ))}
                    </tbody>
                </table>
            )}
            {editable && editUser &&
                <EditProfileModal
                    usuario={editUser}
                    onClose={() => setEditUser(null)}
                    onUpdated={fetchUsuarios}
                    onQueueRefresh={onQueueRefresh}
                />
            }
        </div>
    )
}

