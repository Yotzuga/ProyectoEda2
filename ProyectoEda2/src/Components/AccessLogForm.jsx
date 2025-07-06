import { useState } from 'react'
import { registrarAcceso } from '../api/cliente'
import './AccessLogForm.css'

const zonas = [
    "puerta-vip",
    "puerta-staff",
    "puerta-medico",
    "puerta-general",
    "puerta-ada"
]

export default function AccessLogForm() {
    const [zona, setZona] = useState('')
    const [ts, setTs] = useState('')
    const [msg, setMsg] = useState(null)
    const [loading, setLoading] = useState(false)

    const handleSubmit = async (e) => {
        e.preventDefault()
        setMsg(null)
        if (!zona || !ts) {
            setMsg('Debe seleccionar zona y fecha/hora')
            return
        }
        setLoading(true)
        try {
            // Convierte a segundos
            const res = await registrarAcceso(zona, Math.floor(Date.parse(ts) / 1000))
            if (res.ok) {
                setMsg('Acceso registrado correctamente')
                setZona('')
                setTs('')
            } else {
                setMsg('Error al registrar acceso')
            }
        } catch {
            setMsg('Error al registrar acceso')
        }
        setLoading(false)
    }

    return (
        <form className="access-log-form" onSubmit={handleSubmit}>
            <h4>Registrar Acceso a Zona</h4>
            <label>
                Zona:
                <select value={zona} onChange={e => setZona(e.target.value)}>
                    <option value="">Seleccione...</option>
                    {zonas.map(z => <option key={z} value={z}>{z.replace(/-/g, ' ').replace(/\b\w/g, l => l.toUpperCase())}</option>)}
                </select>
            </label>
            <label>
                Fecha y hora:
                <input type="datetime-local" value={ts} onChange={e => setTs(e.target.value)} />
            </label>
            <button type="submit" disabled={loading}>Registrar acceso</button>
            {msg && <div className="access-log-msg">{msg}</div>}
        </form>
    )
}
