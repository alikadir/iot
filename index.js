import noble from "@abandonware/noble";

const SERVICE_UUID = "ffe0";   // verdiğin +UUID=FFE0
const CHAR_UUID    = "ffe1";   // çoğunlukla FFE1 kullanılır (read/write/notify)

const TARGET_NAME = "RABIA";   // isme göre eşleştirme; ismi bilmiyorsan null yapıp tüm cihazları dinle

console.log("BLE scanning başlatılıyor...");

noble.on("stateChange", async (state) => {
  if (state === "poweredOn") {
    console.log("Adapter hazır, scanning...");
    // belirtirsek sadece bu servis için tarar:
    noble.startScanning([SERVICE_UUID], false);
  } else {
    console.log("Adapter kapalı veya izin yok:", state);
    noble.stopScanning();
  }
});

noble.on("discover", async (peripheral) => {
  try {
    const name = peripheral.advertisement.localName || "<isim yok>";
    console.log("Bulundu:", name, peripheral.uuid);

    if (TARGET_NAME && name !== TARGET_NAME) {
      // isme göre filtre uygulanıyorsa atla
      return;
    }

    // Eğer isme göre filtre yoksa veya isim eşleştiyse bağlan:
    noble.stopScanning();
    console.log("Bağlanılıyor:", name);

    const { address } = peripheral;
    await peripheral.connectAsync();
    console.log("Bağlandı:", name, "(", address, ")");

    // servis ve karakteristikleri bul
    const { characteristics } =
      await peripheral.discoverSomeServicesAndCharacteristicsAsync(
        [SERVICE_UUID],
        [CHAR_UUID]
      );

    if (!characteristics || characteristics.length === 0) {
      console.error("İstenen karakteristik bulunamadı. Tüm karakteristikleri listeleyelim:");
      const all = await peripheral.discoverAllServicesAndCharacteristicsAsync();
      console.log(all.characteristics.map(c => c.uuid));
      await peripheral.disconnectAsync();
      return;
    }

    // FFE1 karakteristiğini bul (küçük harf, tire yok)
    const char = characteristics.find(c => c.uuid === CHAR_UUID);

    if (!char) {
      console.error("FFE1 karakteristiği bulunamadı.");
      await peripheral.disconnectAsync();
      return;
    }

    // Gelen veriyi dinle (notify)
    char.on("data", (data, isNotification) => {
      try {
        console.log("Gelen (BLE -> Node):", data.toString());
      } catch (err) {
        console.log("Gelen ham veri:", data);
      }
    });

    // Subscribe (notify aç)
    await char.subscribeAsync();
    console.log("Notify abone olundu (FFE1).");

    // Fonksiyon: BLE'ye yaz (UART üzerinden modüle gider)
    async function sendToBle(text) {
      if (!text) return;
      const buf = Buffer.from(text); // istersen CRC/terminator ekle
      // many BLE UARTs allow chunking; burada tek seferde yazıyoruz
      await char.writeAsync(buf, false); // false => writeWithoutResponse olabilir, burada response istiyoruz
      console.log("Gönderildi (Node -> BLE):", text);
    }

    // Örnek gönderim:
    await sendToBle("Hello from Node.js!");

    // Terminalden kullanıcı girişini okuyup gönderme (opsiyonel)
    process.stdin.setEncoding("utf8");
    process.stdin.on("data", async (line) => {
      const text = line.trim();
      if (!text) return;
      if (text.toLowerCase() === "exit") {
        console.log("Çıkış yapılıyor...");
        await char.unsubscribeAsync().catch(()=>{});
        await peripheral.disconnectAsync().catch(()=>{});
        process.exit(0);
      }
      await sendToBle(text);
    });

    // Disconnect olayını yakala ve taramayı tekrar başlat
    peripheral.on("disconnect", () => {
      console.log("Cihaz bağlantısı kesildi. Yeniden tarama başlatılıyor...");
      noble.startScanning([SERVICE_UUID], false);
    });

  } catch (err) {
    console.error("Hata:", err);
    // hata sonrası yeniden tarama başlat
    try { noble.startScanning([SERVICE_UUID], false); } catch(e){}
  }
});
