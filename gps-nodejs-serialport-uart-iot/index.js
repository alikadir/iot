import {SerialPort} from 'serialport'
import { ReadlineParser } from '@serialport/parser-readline';
import GPS from 'gps'

const port = new SerialPort({
    path: '/dev/tty.usbserial-0001',
    baudRate: 9600,
});

// Serial port listen data and send GPS library
const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));
parser.on('data', (data) => {
    const rawData = data.toString();
    console.log('Raw GPS Data:', rawData);

    gps.update(rawData); // Send GPS library
});

const gps = new GPS();
gps.on('data', (parsed) => {
    console.log("📍 **Konum Bilgisi**");
    console.log(`   - Enlem: ${parsed.lat}`);
    console.log(`   - Boylam: ${parsed.lon}`);
    
    console.log("\n🕒 **Zaman Bilgisi**");
    console.log(`   - GPS Zamanı: ${parsed.time}`);

    console.log("\n🚀 **Hız ve Yön Bilgisi**");
    console.log(`   - Hız: ${parsed.speed} km/h`);
    console.log(`   - Yön: ${parsed.track}°`);

    console.log("\n📏 **Yükseklik ve Uydu Bilgisi**");
    console.log(`   - Yükseklik: ${parsed.alt} m`);
    console.log(`   - Uydu Sayısı: ${parsed.satellites}`);

    console.log("\n📡 **Sinyal Kalitesi ve Hassasiyet**");
    console.log(`   - HDOP (Konum Hassasiyeti): ${parsed.hdop}`);
    
    console.log("\n🔄 **GPS Sabitleme (Fix) Türü**");
    let fixType = "Bilinmiyor";
    if (parsed.fix === 0) fixType = "❌ No Fix (Konum alınamıyor)";
    if (parsed.fix === 1) fixType = "📍 2D Fix (Sadece enlem & boylam)";
    if (parsed.fix === 2) fixType = "📍📏 3D Fix (Enlem, boylam ve yükseklik)";
    if (parsed.fix === 3) fixType = "📍✅ DGPS Fix (Daha hassas konum)";

    console.log(`   - Fix Türü: ${fixType}`);
});

/*

📍 **Konum Bilgisi**
   - Enlem: 41.0123
   - Boylam: 28.9876

🕒 **Zaman Bilgisi**
   - GPS Zamanı: 2025-02-08T14:35:22.000Z

🚀 **Hız ve Yön Bilgisi**
   - Hız: 15.2 km/h
   - Yön: 120.5°

📏 **Yükseklik ve Uydu Bilgisi**
   - Yükseklik: 45.3 m
   - Uydu Sayısı: 8

📡 **Sinyal Kalitesi ve Hassasiyet**
   - HDOP (Konum Hassasiyeti): 1.2

🔄 **GPS Sabitleme (Fix) Türü**
   - Fix Türü: 📍📏 3D Fix (Enlem, boylam ve yükseklik)

*/



