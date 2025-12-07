import noble from "@abandonware/noble";
import readline from "readline";

const SERVICE_UUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
const RX_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e"; 
const TX_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e"; 

let rxChar = null;

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

noble.on("stateChange", (state) => {
  if (state === "poweredOn") {
    console.log("Scanning...");
    noble.startScanning([SERVICE_UUID], false);
  }
});

noble.on("discover", async (peripheral) => {
  console.log("Device found:", peripheral.advertisement.localName);
  noble.stopScanning();

  await peripheral.connectAsync();
  console.log("Connected!");

  const { characteristics } =
    await peripheral.discoverSomeServicesAndCharacteristicsAsync(
      [SERVICE_UUID],
      [RX_UUID, TX_UUID]
    );

  rxChar = characteristics.find(c => c.uuid === RX_UUID.replace(/-/g, ""));
  const txChar = characteristics.find(c => c.uuid === TX_UUID.replace(/-/g, ""));

  // ESP32 → Node.js (notify)
  txChar.subscribe((err) => {
    if (err) console.log("Subscribe error:", err);
    else console.log("Notifications enabled");
  });

  txChar.on("data", (data) => {
    console.log("\nESP32:", data.toString());
  });

  // Kullanıcıdan terminal input'u → ESP32'ye gönder
  rl.on("line", (input) => {
    if (!rxChar) {
      console.log("RX characteristic hazır değil!");
      return;
    }

    console.log("Gönderiliyor →", input);
    rxChar.write(Buffer.from(input), false);
  });
});
