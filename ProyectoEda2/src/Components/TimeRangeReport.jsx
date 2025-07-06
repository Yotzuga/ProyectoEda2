import { useState } from 'react'
import { getAccesosRango } from '../api/cliente'
import './TimeRangeReport.css'

function mostrarZona(zona) {
    if (!zona) return '-'
    return zona
        .split(/[\s\-]+/)
        .map(p => p.charAt(0).toUpperCase() + p.slice(1))
        .join(' ')
}

export default function TimeRangeReport() {
    const [inicio, setInicio] = useState('')
    const [fin, setFin] = useState('')
    const [result, setResult] = useState([])
    const [msg, setMsg] = useState(null)
    const [loading, setLoading] = useState(false)
    const [pagina, setPagina] = useState(0)
    const porPagina = 20

    const handleSubmit = async (e) => {
        e.preventDefault()
        setMsg(null)
        setResult([])
        if (!inicio || !fin) {
            setMsg('Debe ingresar ambos valores')
            return
        }
        const iniTs = Date.parse(inicio) / 1000
        const finTs = Date.parse(fin) / 1000
        if (isNaN(iniTs) || isNaN(finTs)) {
            setMsg('Formato de fecha inválido')
            return
        }
        setLoading(true)
        try {
            const data = await getAccesosRango(iniTs, finTs)
            setResult(data)
        } catch {
            setMsg('Error al consultar accesos')
        }
        setLoading(false)
    }

    return (
        <div className="time-range-report">
            <form onSubmit={handleSubmit}>
                <label>
                    Inicio:<br />
                    <input
                        type="datetime-local"
                        value={inicio}
                        onChange={e => setInicio(e.target.value)}
                    />
                </label>
                <label>
                    Fin:<br />
                    <input
                        type="datetime-local"
                        value={fin}
                        onChange={e => setFin(e.target.value)}
                    />
                </label>
                <button type="submit" disabled={loading}>Consultar</button>
            </form>
            {msg && <div className="time-range-msg">{msg}</div>}
            {result.length > 0 && (
                <div className="time-range-result">
                    <h4>Accesos en rango:</h4>
                    <ul>
                        {result.slice(pagina * porPagina, (pagina + 1) * porPagina).map((a, i) => (
                            <li key={pagina * porPagina + i}>
                                Zona: <b>{mostrarZona(a.zona)}</b> | Fecha: {new Date(a.ts * 1000).toLocaleString()}
                            </li>
                        ))}
                    </ul>
                    <div style={{ textAlign: 'center', marginTop: '1rem' }}>
                        <button
                            onClick={() => setPagina(p => Math.max(0, p - 1))}
                            disabled={pagina === 0}
                        >Anterior</button>
                        <span style={{ margin: '0 1rem' }}>
                            Página {pagina + 1} de {Math.ceil(result.length / porPagina)}
                        </span>
                        <button
                            onClick={() => setPagina(p => p + 1)}
                            disabled={(pagina + 1) * porPagina >= result.length}
                        >Siguiente</button>
                    </div>
                </div>
            )}
        </div>
    )
}
