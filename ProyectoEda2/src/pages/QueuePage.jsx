import { useState } from 'react'
import QueueTop5 from '../components/QueueTop5'
import ProcessNextButton from '../components/ProcessNextButton'

export default function QueuePage() {
    const [refreshSignal, setRefreshSignal] = useState(0)
    const forceRefresh = () => setRefreshSignal(s => s + 1)

    return (
        <div>
            <h2>Cola de Prioridad</h2>
            <QueueTop5 refreshSignal={refreshSignal} />
            <ProcessNextButton onProcessed={forceRefresh} />
        </div>
    )
}


