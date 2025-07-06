import TopZoneReport from '../components/TopZoneReport'
import TimeRangeReport from '../components/TimeRangeReport'
import AccessLogForm from '../components/AccessLogForm'

export default function ReportsPage() {
    return (
        <div>
            <h2>Reportes y Estadísticas</h2>
            <TopZoneReport />
            <TimeRangeReport />
            <hr />
            <AccessLogForm />
        </div>
    )
}
