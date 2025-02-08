import {SerialPort} from 'serialport'
import { ReadlineParser } from '@serialport/parser-readline';
import GPS from 'gps'

const port = new SerialPort({
    path: '/dev/tty.usbserial-0001',
    baudRate: 9600,
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

const gps = new GPS();

parser.on('data', (data) => {
    const rawData = data.toString();
    console.log('Raw GPS Data:', rawData);

    gps.update(rawData); // GPS verisini işle
});

gps.on('data', (parsed) => {
    console.log(`📍 Konum: ${parsed.lat}, ${parsed.lon}`);
    console.log(`🕒 Zaman: ${parsed.time}`);
});
