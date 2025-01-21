import { SerialPort } from 'serialport'
import readline from 'node:readline';

// Create Console Read to read command 
const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});

// Create a port
const port = new SerialPort({
  path: '/dev/tty.wchusbserial2140',
  baudRate: 115200,
});


port.on('data', function (incomingByteArrayData) {
  const incomingText = Buffer.from((incomingByteArrayData)).toString('utf-8');
  console.log('Receive Data from IoT: ', incomingText);
});

const readConsoleSendIoT = () => {
  rl.question(`What do you send to IoT device via SerialPort (Empty to read): `, command => {
    console.log(`Send Command to IoT: ${command}`);
    if(command == '') 
      port.write('read');
    else
      port.write(command);
    //rl.close();
    setTimeout(readConsoleSendIoT,3000);
  });
}

readConsoleSendIoT();

