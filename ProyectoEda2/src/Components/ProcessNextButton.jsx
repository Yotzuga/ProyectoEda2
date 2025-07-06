import { useState } from 'react'
import { extraerSiguienteCola } from '../api/cliente'
import './ProcessNextButton.css'

export default function ProcessNextButton({ onProcessed }) {
    const [msg, setMsg] = useState(null)
    const [info, setInfo] = useState(null)
    const [loading, setLoading] = useState(false)

    const handleClick = async () => {
        setMsg(null)
        setInfo(null)
        setLoading(true)
        try {
            const res = await extraerSiguienteCola()
            if (!res) {
                setMsg('No hay usuarios en cola')
            } else {
                setInfo(res)
                if (onProcessed) onProcessed()
            }
        } catch {
            setMsg('Error al atender siguiente')
        }
        setLoading(false)
    }

    return (
        <div className="process-next-btn">
            <button onClick={handleClick} disabled={loading}>
                Atender Siguiente
            </button>
            {msg && <div className="process-next-msg">{msg}</div>}
            {info && (
                <div className="process-next-info">
                    <b>Atendido:</b> DNI {info.dni}, prioridad {info.prioridad}, fecha {new Date(info.ts * 1000).toLocaleString()}
                </div>
            )}
        </div>
    )
}
