import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
    plugins: [react()],
    server: {
        // 1. Otwieraj przegl¹darkê automatycznie po wpisaniu 'npm run dev'
        open: true,
        // 2. Konfiguracja Proxy (Tunel do Backendu)
        proxy: {
            '/api': {
                target: 'http://localhost:5202', // Tu wpisujemy port Twojego API z C#
                changeOrigin: true,
                secure: false, // Wa¿ne! Pozwala ³¹czyæ siê mimo certyfikatu "localhost"
            }
        }
    },
})