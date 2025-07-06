import { useState } from 'react'
import { registrarUsuario } from '../api/cliente'
import './RegisterUserForm.css'

const perfiles = [
    { value: "vip", label: "VIP" },
    { value: "personal-medico", label: "Personal Medico" },
    { value: "seguridad", label: "Seguridad" },
    { value: "discapacitados", label: "Discapacitados" },
    { value: "publico-general", label: "Publico General" }
]

export default function RegisterUserForm() {
    const [dni, setDni] = useState('')
    const [perfil, setPerfil] = useState('')
    const [msg, setMsg] = useState(null)
    const [loading, setLoading] = useState(false)

    const handleSubmit = async (e) => {
        e.preventDefault()
        setMsg(null)
        if (!dni || !perfil) {
            setMsg("Debe ingresar DNI y seleccionar perfil.")
            return
        }
        setLoading(true)
        const res = await registrarUsuario(Number(dni), perfil)
        if (res.status === 201) {
            setMsg("Usuario registrado correctamente.")
            setDni('')
            setPerfil('')
        } else if (res.status === 409) {
            setMsg("El usuario ya existe.")
        } else {
            setMsg("Error al registrar usuario.")
        }
        setLoading(false)
    }

    return (
        <form className="register-user-form" onSubmit={handleSubmit}>
            <label>
                DNI:
                <input
                    type="number"
                    value={dni}
                    onChange={e => setDni(e.target.value)}
                    min="1"
                    required
                />
            </label>
            <label>
                Perfil:
                <select value={perfil} onChange={e => setPerfil(e.target.value)} required>
                    <option value="">Seleccione...</option>
                    {perfiles.map(p => <option key={p.value} value={p.value}>{p.label}</option>)}
                </select>
            </label>
            <button type="submit" disabled={loading}>Registrar</button>
            {msg && <div className="register-user-msg">{msg}</div>}
        </form>
    )
}
