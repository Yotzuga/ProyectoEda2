import { useEffect, useState, useRef } from 'react'
import { getTop5Cola } from '../api/cliente'
import './QueueTop5.css'

export default function QueueTop5({ refreshSignal }) {
    const [top, setTop] = useState([])
    const [loading, setLoading] = useState(true)
    const intervalRef = useRef(null)

    const fetchTop = async () => {
        try {
            const data = await getTop5Cola()
            setTop(data) // No ordenar, solo mostrar lo que viene del backend
            setLoading(false)
        } catch {
            setLoading(false)
        }
    }

    useEffect(() => {
        fetchTop()
        intervalRef.current = setInterval(fetchTop, 4000)
        return () => clearInterval(intervalRef.current)
    }, [])

    // Permite refresco externo (cuando cambie refreshSignal)
    useEffect(() => {
        if (refreshSignal !== undefined) fetchTop()
        // eslint-disable-next-line
    }, [refreshSignal])

    function mostrarPerfil(perfil) {
        if (!perfil) return '-'
        return perfil
            .split(/[\s\-]+/)
            .map(p => p.charAt(0).toUpperCase() + p.slice(1))
            .join(' ')
    }

    return (
        <div className="queue-top5">
            <h3>Top 5 en Cola</h3>
            {loading ? <div>Cargando...</div> : (
                <table>
                    <thead>
                        <tr>
                            <th>#</th>
                            <th>DNI</th>
                            <th>Perfil</th>
                            <th>Fecha llegada</th>
                        </tr>
                    </thead>
                    <tbody>
                        {top.map((item, idx) => (
                            <tr key={item.dni}>
                                <td>{idx + 1}</td>
                                <td>{item.dni}</td>
                                <td>{mostrarPerfil(item.perfil)}</td>
                                <td>{new Date(item.ts * 1000).toLocaleString()}</td>
                            </tr>
                        ))}
                    </tbody>
                </table>
            )}
        </div>
    )
}

