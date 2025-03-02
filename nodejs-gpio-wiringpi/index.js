import Gpio from './gpio-wiringpi.js';


// Show pins with sh command "gpio readall"

/* 
// Raspberry Pi
let gpioRed = new Gpio({pin:27});
let gpioYellow = new Gpio({pin:28});
let gpioGreen = new Gpio({pin:29});
*/

// Orange Pi
let gpioRed = new Gpio({pin:13});
let gpioYellow = new Gpio({pin:15});
let gpioGreen = new Gpio({pin:16});

gpioRed.write(0);
gpioYellow.write(0);
gpioGreen.write(0);

const redLight = () => {
  gpioYellow.write(1);
  gpioGreen.write(0);
  sleep(2000).then(()=> { gpioYellow.write(0); gpioRed.write(1); });
}

const greenLight = () => {
  gpioYellow.write(1);
  sleep(3000).then(()=> { gpioYellow.write(0); gpioRed.write(0); gpioGreen.write(1); });
}

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}
                                                                                                                                                                                                                                                                                                                                    
setInterval(function(){                                                                                                                                           
    redLight();                                                                                                                                               
    setTimeout(function() { greenLight(); }, 5000);                                                                                                               
},10000); 
