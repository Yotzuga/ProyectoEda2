import UserSearch from '../components/UserSearch'
import UserList from '../components/UserList'

export default function UserManagementPage() {
    return (
        <div>
            <h2>Gestión de Usuarios</h2>
            <UserSearch />
            <hr />
            <UserList /> {/* Editable por defecto aquí */}
        </div>
    )
}
