import { useState } from 'react'
import { getZonaTop } from '../api/cliente'
import './TopZoneReport.css'

export default function TopZoneReport() {
    const [zona, setZona] = useState(null)
    const [loading, setLoading] = useState(false)
    const [msg, setMsg] = useState(null)

    const handleClick = async () => {
        setLoading(true)
        setMsg(null)
        try {
            const z = await getZonaTop()
            setZona(z)
        } catch {
            setMsg('Error al consultar zona top')
        }
        setLoading(false)
    }

    return (
        <div className="top-zone-report">
            <button onClick={handleClick} disabled={loading}>
                Mostrar Zona Top
            </button>
            {zona && <div className="top-zone-result">Zona con más accesos: <b>{zona}</b></div>}
            {msg && <div className="top-zone-msg">{msg}</div>}
        </div>
    )
}
