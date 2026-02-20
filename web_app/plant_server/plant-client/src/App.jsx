// 1. Importujemy Twój nowy komponent
import PlantDashboard from './components/PlantDashboard'

// 2. Opcjonalnie: Importujemy style (jeœli chcesz zachowaæ globalne formatowanie)
import './App.css'

function App() {
    // 3. Zamiast licznika i loga, zwracamy Twój Dashboard
    return (
        <PlantDashboard />
    )
}

export default App