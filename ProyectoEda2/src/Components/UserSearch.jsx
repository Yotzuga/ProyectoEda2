import { useState } from 'react'
import { buscarUsuario, actualizarPerfil, actualizarPrioridadCola } from '../api/cliente'
import './UserSearch.css'

const perfiles = [
    { value: "vip", label: "VIP" },
    { value: "personal-medico", label: "Personal Medico" },
    { value: "seguridad", label: "Seguridad" },
    { value: "discapacitados", label: "Discapacitados" },
    { value: "publico-general", label: "Publico General" }
]

function EditProfileModal({ usuario, onClose, onUpdated }) {
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
            // 2. Si está en cola, actualizar prioridad en heap
            if (usuario.enCola) {
                const res2 = await actualizarPrioridadCola(usuario.dni, perfil)
                if (!res2.ok) {
                    setMsg('Error al actualizar prioridad en cola')
                    setLoading(false)
                    return
                }
                if (onQueueRefresh) onQueueRefresh()
            }
            setMsg('Perfil actualizado')
            onUpdated(perfil)
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

export default function UserSearch({ onQueueRefresh }) {
    const [dni, setDni] = useState('')
    const [result, setResult] = useState(null)
    const [msg, setMsg] = useState(null)
    const [loading, setLoading] = useState(false)
    const [editOpen, setEditOpen] = useState(false)
    const [perfilActual, setPerfilActual] = useState('')

    const handleSearch = async (e) => {
        e.preventDefault()
        setMsg(null)
        setResult(null)
        setEditOpen(false) // Cierra el modal si se busca un nuevo usuario
        if (!dni) {
            setMsg('Ingrese un DNI')
            return
        }
        setLoading(true)
        try {
            const res = await buscarUsuario(dni)
            setResult(res)
            setPerfilActual(res.perfil)
            setMsg(null)
        } catch {
            setMsg('Error al buscar usuario')
        }
        setLoading(false)
    }

    const handleUpdated = (nuevoPerfil) => {
        setPerfilActual(nuevoPerfil)
        setResult(r => ({ ...r, perfil: nuevoPerfil }))
    }

    return (
        <>
            <form className="user-search-form" onSubmit={handleSearch}>
                <label>
                    Buscar por DNI:
                    <input
                        type="number"
                        value={dni}
                        onChange={e => setDni(e.target.value)}
                        min="1"
                        style={{ marginLeft: '0.5rem' }}
                    />
                </label>
                <button type="submit" disabled={loading} style={{ marginLeft: '1rem' }}>Buscar</button>
                {msg && <div className="user-search-msg">{msg}</div>}
                {result && (
                    <div className="user-search-result" style={{ marginTop: '1rem' }}>
                        {result.valid
                            ? <>
                                <div><b>Perfil:</b> {mostrarPerfil(perfilActual)}</div>
                                {typeof result.enCola !== "undefined" && (
                                    <div>
                                        <b>En cola:</b> {result.enCola ? "Sí" : "No"}<br />
                                        <b>Atendido:</b> {result.atendido ? "Sí" : "No"}
                                    </div>
                                )}
                                <button
                                    type="button"
                                    onClick={() => setEditOpen(true)}
                                    disabled={editOpen}
                                >
                                    Editar perfil/prioridad
                                </button>
                            </>
                            : <div>Usuario no encontrado</div>
                        }
                    </div>
                )}
            </form>
            {editOpen &&
                <EditProfileModal
                    usuario={{ dni, perfil: perfilActual, enCola: result?.enCola }}
                    onClose={() => setEditOpen(false)}
                    onUpdated={handleUpdated}
                />
            }
        </>
    )
}
