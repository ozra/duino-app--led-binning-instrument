## 0.1.1 (2017-04-24) ##

* current measurement of LED and "lightmeter" PV output (deliberately 
  shortcircuited)
* voltage sweeping via X9C104 digital pot and LM317 voltage reg
   * the 104 is way high in resistance, a composite series/parallel 
     resistor network yielding a logaritmish voltage range is used to
     interface reasonably with the LM317 (which actually has merits too)
* The ACS712 current meters, while non-intrusive using hall-effect 
  sensing, are _highly_ inexact for currents as low as in this 
  application
   * Will therefor resort to the old method of using 0.1 Ohm shunts and
     op-amps for measurement - simply incorporating the circuit offset
     in to the calculations.

## 0.1.0 (2017-04-21) ##

* Initialized project using skelett
* First commit
