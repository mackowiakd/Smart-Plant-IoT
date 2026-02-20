import React, { useEffect, useState } from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer } from 'recharts';
import axios from 'axios';

const PlantDashboard = () => {
    const [data, setData] = useState([]);
    const [current, setCurrent] = useState(null);

    const API_URL = '/api/plant';

    const fetchData = async () => {
        try {
            const historyRes = await axios.get(`${API_URL}/history`);

            const formattedData = historyRes.data.map(item => ({
                ...item,
                time: new Date(item.timestamp).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit', second: '2-digit' })
            }));
            setData(formattedData);

            if (formattedData.length > 0) {
                setCurrent(formattedData[formattedData.length - 1]);
            }
        } catch (error) {
            console.error("Błąd pobierania danych. Czy serwer C# działa?", error);
        }
    };

    useEffect(() => {
        fetchData();
        const interval = setInterval(fetchData, 2000);
        return () => clearInterval(interval);
    }, []);

    return (
        <div className="p-8 bg-slate-50 min-h-screen font-sans">
            <header className="mb-8 flex items-center justify-between">
                <div>
                    <h1 className="text-3xl font-bold text-slate-800">🌱 Moja Inteligentna Doniczka</h1>
                    <p className="text-slate-500">Monitoring parametrów życiowych rośliny</p>
                </div>
                <div className="text-right">
                    <span className={`px-3 py-1 rounded-full text-sm font-medium ${current ? 'bg-green-100 text-green-700' : 'bg-gray-100 text-gray-500'}`}>
                        {current ? '● Online' : '○ Oczekiwanie na dane...'}
                    </span>
                </div>
            </header>

            {/* 1. Główny komunikat na górze strony */}
            {current?.status && (
                <div style={{
                    backgroundColor: '#fef3c7',
                    color: '#92400e',
                    padding: '12px 20px',
                    borderRadius: '10px',
                    marginBottom: '30px',
                    border: '1px solid #fcd34d',
                    fontWeight: 'bold',
                    textAlign: 'center',
                    boxShadow: '0 2px 4px rgba(0,0,0,0.05)'
                }}>
                    📢 AKTUALNY STAN: {current.status}
                </div>
            )}

            {/* Kafelki z aktualnym stanem */}
            <div style={{ display: 'flex', gap: '20px', marginBottom: '30px', flexWrap: 'wrap' }}>
                <div style={{ flex: '1', minWidth: '180px', background: 'white', padding: '20px', borderRadius: '15px', border: '1px solid #e2e8f0', textAlign: 'center', boxShadow: '0 2px 4px rgba(0,0,0,0.05)' }}>
                    <h3 style={{ fontSize: '12px', color: '#94a3b8', textTransform: 'uppercase', fontWeight: 'bold' }}>Wilgotność Gleby</h3>
                    <p style={{ fontSize: '32px', fontWeight: 'bold', color: '#2563eb', margin: '10px 0' }}>{current?.moisture || 0}%</p>
                </div>

                <div style={{ flex: '1', minWidth: '180px', background: 'white', padding: '20px', borderRadius: '15px', border: '1px solid #e2e8f0', textAlign: 'center', boxShadow: '0 2px 4px rgba(0,0,0,0.05)' }}>
                    <h3 style={{ fontSize: '12px', color: '#94a3b8', textTransform: 'uppercase', fontWeight: 'bold' }}>Natężenie Światła</h3>
                    <p style={{ fontSize: '32px', fontWeight: 'bold', color: '#eab308', margin: '10px 0' }}>{current?.lux || 0} lx</p>
                </div>

                <div style={{ flex: '1', minWidth: '180px', background: 'white', padding: '20px', borderRadius: '15px', border: '1px solid #e2e8f0', textAlign: 'center', boxShadow: '0 2px 4px rgba(0,0,0,0.05)' }}>
                    <h3 style={{ fontSize: '12px', color: '#94a3b8', textTransform: 'uppercase', fontWeight: 'bold' }}>Temperatura</h3>
                    <p style={{ fontSize: '32px', fontWeight: 'bold', color: '#ef4444', margin: '10px 0' }}>{current?.temperature || 0}°C</p>
                </div>

                <div style={{ flex: '1', minWidth: '180px', background: 'white', padding: '20px', borderRadius: '15px', border: '1px solid #e2e8f0', textAlign: 'center', boxShadow: '0 2px 4px rgba(0,0,0,0.05)' }}>
                    <h3 style={{ fontSize: '12px', color: '#94a3b8', textTransform: 'uppercase', fontWeight: 'bold' }}>Wilgotność Pow.</h3>
                    <p style={{ fontSize: '32px', fontWeight: 'bold', color: '#0ea5e9', margin: '10px 0' }}>{current?.humidity || 0}%</p>
                </div>

                <div style={{ flex: '1', minWidth: '180px', background: 'white', padding: '20px', borderRadius: '15px', border: '1px solid #e2e8f0', textAlign: 'center', boxShadow: '0 2px 4px rgba(0,0,0,0.05)' }}>
                    <h3 style={{ fontSize: '12px', color: '#94a3b8', textTransform: 'uppercase', fontWeight: 'bold' }}>Ciśnienie</h3>
                    <p style={{ fontSize: '32px', fontWeight: 'bold', color: '#64748b', margin: '10px 0' }}>{current?.pressure || 0} hPa</p>
                </div>
            </div>
            {/* Sekcja Wykresu - dodajemy nad tabelą */}
            <div style={{
                backgroundColor: 'white',
                padding: '20px',
                borderRadius: '16px',
                border: '1px solid #e2e8f0',
                marginBottom: '32px',
                boxShadow: '0 1px 3px rgba(0,0,0,0.1)'
            }}>
                <h3 style={{ fontSize: '18px', fontWeight: 'bold', marginBottom: '20px', color: '#334155' }}>
                    Trend wilgotności i temperatury
                </h3>
                <div style={{ width: '100%', height: '300px' }}>
                    <ResponsiveContainer width="100%" height="100%">
                        <LineChart data={data.slice(-20)}> {/* Pokazuje ostatnie 20 pomiarów */}
                            <CartesianGrid strokeDasharray="3 3" vertical={false} stroke="#f1f5f9" />
                            <XAxis
                                dataKey="time"
                                fontSize={12}
                                tick={{ fill: '#94a3b8' }}
                                axisLine={false}
                                tickLine={false}
                            />
                            <YAxis
                                fontSize={12}
                                tick={{ fill: '#94a3b8' }}
                                axisLine={false}
                                tickLine={false}
                            />
                            <Tooltip
                                contentStyle={{ borderRadius: '10px', border: 'none', boxShadow: '0 4px 6px rgba(0,0,0,0.1)' }}
                            />
                            <Legend />
                            <Line
                                type="monotone"
                                dataKey="moisture"
                                stroke="#2563eb"
                                name="Wilgotność (%)"
                                strokeWidth={3}
                                dot={{ r: 4 }}
                                activeDot={{ r: 6 }}
                            />
                            <Line
                                type="monotone"
                                dataKey="temperature"
                                stroke="#ef4444"
                                name="Temperatura (°C)"
                                strokeWidth={3}
                                dot={{ r: 4 }}
                            />
                        </LineChart>
                    </ResponsiveContainer>
                </div>
            </div>
            {/* Sekcja tabeli z historią */}
            <div className="mt-8 bg-white p-6 rounded-2xl shadow-sm border border-slate-100">
                <div className="flex items-center justify-between mb-4">
                    <h3 className="text-lg font-semibold text-slate-700">Lista ostatnich wpisów</h3>

                    {/* 2. DODATKOWY KOMUNIKAT BEZPOŚREDNIO NAD TABELĄ */}
                    {current?.status && (
                        <span className="text-sm font-bold px-3 py-1 bg-amber-50 text-amber-700 border border-amber-200 rounded-lg animate-pulse mr-10">
                            Ostatni status: {current.status}
                        </span>
                    )}
                </div>

                <div className="overflow-x-auto">
                    <table className="w-full text-left text-sm text-slate-600 border-collapse">
                        <thead>
                            <tr className="border-b text-slate-400 uppercase text-xs tracking-wider">
                                <th className="py-3">Godzina</th>
                                <th className="py-3">Wilgotność gleby</th>
                                <th className="py-3">Światło</th>
                                <th className="py-3">Temperatura</th>
                                <th className="py-3">Wilgotność powietrza</th>
                                <th className="py-3">Ciśnienie</th>
                            </tr>
                        </thead>
                        <tbody>
                            {data.slice(-10).reverse().map((pomiar, idx) => (
                                <tr key={idx} className="border-b last:border-0 hover:bg-slate-50 transition-colors">
                                    <td className="py-3 font-medium text-slate-900">{pomiar.time}</td>
                                    <td className="py-3 text-blue-600 font-semibold">{pomiar.moisture}%</td>
                                    <td className="py-3 text-yellow-600 font-semibold">{pomiar.lux} lx</td>
                                    <td className="py-3 text-red-600 font-semibold">{pomiar.temperature}°C</td>
                                    <td className="py-3 text-sky-600 font-semibold">{pomiar.humidity}%</td>
                                    <td className="py-3 text-gray-600 font-semibold">{pomiar.pressure} hPa</td>
                                </tr>
                            ))}
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    );
};

export default PlantDashboard;