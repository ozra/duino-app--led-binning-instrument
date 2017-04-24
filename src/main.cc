#define DEBUG

#include "OnyxTypeAliases.h"
#include "TerseConvenienceKeywords.h"
#include "DebugLogger.h" // *TODO* ?

#include "HolyDio.h"

#include "Dsp.h"  // *TODO* pure fucking FOO FILE!
#include "DspEma.h"

/* #include "Serial.h" // *TODO* */
#include "Arduino.h" // *TODO*

/* #include "ChronoTrigger.h" // *TODO* only for prototyping classes in here (DON'T BTW !×!) */


let const LOAD_CURRENT_PIN = PinNumber(0);
let const REG_VOLT_PIN = PinNumber(1);
let const PV_CURRENT_PIN = PinNumber(2);

let ledCurrentEma =  Dsp::Ema<F32, false>(1000);
let pvCurrentEma =  Dsp::Ema<F32, false>(1000);
/* let slowEma =     Dsp::Ema<F32, false>(14); */
let samples =     0UL;

fn readLedCurrent() -> F32 {
   let const bias_calib = 270;//-111.0; // Still gives about -5ma error biase, but one step up gives +30mA error - when resolving here at the int-level
   let const scale_calib = 1.433;
   let const mVPerAmp = 185;
   let const mVRef = 5000.0;
   let const mA_p_V_p_U = (mVRef / 1023.0) / mVPerAmp * scale_calib * 1000;
   let const bias = (-511 * mA_p_V_p_U) + bias_calib;

   let reading = analogRead(LOAD_CURRENT_PIN);
   let result = reading * mA_p_V_p_U + bias;
   samples += 1;

   return result;
}

fn readPVCurrent() -> F32 {
   let const bias_calib = 260;//-111.0; // Still gives about -5ma error biase, but one step up gives +30mA error - when resolving here at the int-level
   let const scale_calib = 1.433;
   let const mVPerAmp = 50;
   let const mVRef = 5000.0;
   let const mA_p_V_p_U = (mVRef / 1023.0) / mVPerAmp * scale_calib * 1000;
   let const bias = (-511 * mA_p_V_p_U) + bias_calib;

   let reading = analogRead(PV_CURRENT_PIN);
   let result = reading * mA_p_V_p_U + bias;
   samples += 1;

   return result;
}

fn step_r_wiper() -> Void {
   dioWrite<2>(1);
   delayMicroseconds(5000);
   dioWrite<2>(0);
   delayMicroseconds(5000);
}

fn setup_programmable_voltage() -> Void {
   dioPinMode<2>(1);
   dioPinMode<3>(1);
   dioPinMode<4>(1);

   dioWrite<2>(0);
   dioWrite<3>(0);
   dioWrite<4>(0);

   for(let i = 100; i != 0; --i) {
      step_r_wiper();
   }

   dioWrite<3>(1);
}

fn prime_current_emas() -> Void {
   ledCurrentEma.prime(readLedCurrent());
   pvCurrentEma.prime(readPVCurrent());
   for(let i = 5000; i != 0; --i) {
      ledCurrentEma << readLedCurrent();
      pvCurrentEma << readPVCurrent();
   }
   /* slowEma.prime(ledCurrentEma()); */
}

fn setup() -> Void {
   Serial.begin(115200);            // Initialize serial communications with the PC
   say("Kick Off!\n\r");

   setup_programmable_voltage();
   prime_current_emas();
}

/* let const TOP = 99; */
let const TOP = 25;
let const LOOP_DELAY = 1000; //300

fn loop() -> Void {
   static let mtime = millis();
   static let r_counter = -1;
   static let r_dir = 1;

   let li = ledCurrentEma << readLedCurrent();
   let pi = pvCurrentEma << readPVCurrent();

   let regVolt = analogRead(REG_VOLT_PIN) * (5.0 / 1023);

   if (millis() - mtime > LOOP_DELAY) {

      step_r_wiper();

      /* slowEma << i; */

      let ledP = regVolt * li;

      say(", LED: ");
      say(regVolt);

      say("V, ");
      say(li);
      /* say(" ("); */
      /* say(slowEma()); */
      /* say(")"); */

      say("mA, ");
      say(ledP);

      say("mW,  PV: ");
      say(pi);

      say("mA, ratio = ");
      say(pi / ledP);

      say(", n = ");
      say(samples);

      say(", i = ");
      say(r_counter);

      say("\n\r");

      r_counter += r_dir;
      if (r_counter >= TOP) {
         dioWrite<3>(0);
         r_dir = -1;
         say("\n\rtop reached\n\r\n\r");

      } else if (r_counter <= 0) {
         dioWrite<3>(1);
         r_dir = 1;
         say("\n\rbottom reached\n\r\n\r");
      }

      mtime = millis();
      samples = 0;
   }

   /* delay(5); */
}

