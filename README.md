# Heat exchanger
Heat exchanger design and implementation for a single vent solution

I wanted to improve ventilation but not introduce a high loss of energy.
When ventilating the expelled air is heated and the air replacing it from outside is cool, to improve efficiency one wants to exchange the heat so the air ejected heats the air added.
Normally this is done in a central unit with one or two valves that sucks air out and 1 or more valves that sends new air in.
This central unit then has a heat exchanger that is heated by the air going out, and then the air going in is heated by this element.

In an  old house adding a centralized unit for heat exchange is difficult as the duct-work is not in place. Especially in the basement of a multistory building. For the top floor with an attic it is simpler to do the duct-work on the attic.

To solve this one can use a heat exchange unit/design described here that is inserted in standard ventilation vents in the house.
Since this design will localize the complete unit to one vent the design does not have a separate path for in and out of the air, but the same place/vent is used for both.
This is solved by having the unit work in cycles where it ejects air for a set time then injects air for a period, the air then goes out through the heat exchanger warming it up, then air goes in through the heat exchanger and gets warmed up by the heat exchanger element.
For a balanced system multiple units in multiple vents should be synchronized so when one unit injects air the other ejects air so the air flow/balance is neutral.

![Ceramic heat exchanger element](/doc/heat_exchanger_ceramics.png)

The solution exists in the market but they are expensive or do not have the control enabled via wifi.
Cheapest solution found commercial without any smart control or sensing was ~350us

So the solution should be cheep, and enable collecting data and control for on prem only, no external cloud needed.

The solution was brought about when I found that one can get these ceramic heat exchangers from Aliexpress (and others) for cheap ~30us. These seems to be Euro 1 catalytic converters.

Shown in the picture is the one I received.

Next needed in the design is to push air in both directions, the commercial alternatives uses a fan that goes both directions, but as that will not result in a good fan I opted to use two fans made for cooling computers, as they are quiet.
So my design uses one fan on each side of the heat exchanger pushing air in or out through the heat exchanger element.
To control the fans and measure the temperatures a wifi enabled micro-controller was added. The prototype uses a raspberry pico W for this, with code in micropython.


BOM for the prototype:
- [Ceramic heat exchanger](https://www.aliexpress.com/w/wholesale-ceramic-heat-exchanger.html?spm=a2g0o.tm1000006196.search.0)
- Raspberry pico W
- Noctua NF-S12A FLX 120mm fan.
- FDP8880 Mosfet, Power Field-Effect Transistor.
- 10k termistor.
- 10k resistor.
- Breadboard
- Dupont cables
- 12V to 3.3V DC-DC supply module

![Prototype HW](/doc/prototype_bb.png)