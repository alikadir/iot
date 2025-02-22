import * as tf from '@tensorflow/tfjs-node';
import * as cocoSsd from '@tensorflow-models/coco-ssd';
import fs from 'fs';
import { spawn } from 'child_process';
import { Gpio } from 'onoff';


const ledGreen = new Gpio(529, 'out'); // GPIO17-529
const ledYellow = new Gpio(539, 'out'); // GPIO27-539
const ledRed = new Gpio(534, 'out'); // GPIO22-534

async function captureImage() {
    return new Promise((resolve, reject) => {
        const filePath = 'animal/animal.jpg';
        const raspistill = spawn('rpicam-still', ['-o', filePath, '-t', '1']);

        raspistill.on('close', (code) => {
            if (code === 0) {
                resolve(filePath);
            } else {
                reject(new Error('Failed to capture image'));
            }
        });
    });
}

function turnOnLeds(){
    ledGreen.writeSync(1);
    ledYellow.writeSync(1);
    ledRed.writeSync(1);
}

function turnOffLeds(){
    ledGreen.writeSync(0);
    ledYellow.writeSync(0);
    ledRed.writeSync(0);
}

async function detectObjects() {
                                                                                                                                    
    const model = await cocoSsd.load();                                                                                             
                                                                                                                                    
    setInterval(async () => {                                                                                                       
                                                                                                                                    
        turnOnLeds();                                                                                                               
                                                                                                                                    
        try {                                                                                                                       
            const imagePath = await captureImage();                                                                                 
            const imageBuffer = fs.readFileSync(imagePath);                                                                         
            const imageTensor = tf.node.decodeImage(imageBuffer);                                                                   
            const predictions = await model.detect(imageTensor);                                                                    
                                                                                                                                    
            predictions.forEach((prediction) => {                                                                                   
                if (['person', 'dog', 'cat', 'bear','horse'].includes(prediction.class)) {                                          
                    console.log(                                                                                                    
                        `Detected: ${prediction.class} with ${Math.round(prediction.score * 100)}% confidence`,                     
                    );                                                                                                              
                }                                                                                                                   
            });                                                                                                                     
        } catch (error) {                                                                                                           
            console.error('Error detecting objects:', error);                                                                       
        }                                                                                                                           
                                                                                                                                    
        turnOffLeds();                                                                                                              
                                                                                                                                    
    }, 3000);                                                                                                                       
}

(async function () {
    detectObjects();
})();
