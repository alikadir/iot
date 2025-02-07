import {SerialPort} from 'serialport'

// Create a port
const port = new SerialPort({
    path: '/dev/tty.usbserial-0001',
    baudRate: 9600,
});

// Receiver
port.on('data', function (incomingByteArrayData) {
    const incomingText = Buffer.from((incomingByteArrayData)).toString('utf-8');
    console.log('Receive Data from IoT: ', incomingText);
});

// Transmitter
const sendData = () => {
    const messageBytes = Buffer.from([
        0x0/* first digit of target lora address */,
        0x3F/* second digit of target lora address */,
        0x17/* lora modules must be same channel */,
        0x00,
        0x00,
        0x01,
        0x04/* message block 00145300 :) */,
        0x05,
        0x03,
        0x00,
        0x00])
    port.write(messageBytes);
    console.log('Send data to 0x3f=63 address and 0x17=23 channel, other data=00145300')
}

setInterval(sendData, 1000);
