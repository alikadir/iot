import { exec } from 'child_process';

class Gpio {
    constructor({ pin = 5, mode = 'out', ready = () => {} }) {
        this.pin = pin;
        this.mode = mode;

        this.init().then(() => ready());
    }

    async init() {
        return this.cmd(`gpio mode ${this.pin} ${this.mode}`);
    }

    async read() {
        const state = await this.cmd(`gpio read ${this.pin}`);
        return state.replace(/[^\d]/gm, '');
    }

    async write(value) {
        return this.cmd(`gpio write ${this.pin} ${value}`);
    }

    cmd(command) {
        return new Promise((resolve, reject) => {
            exec(command, (error, stdout, stderr) => {
                if (error) {
                    reject(error);
                }
                resolve(stdout);
            });
        });
    }
}

export default Gpio;
