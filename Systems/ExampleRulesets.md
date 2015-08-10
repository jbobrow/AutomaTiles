AutomaTile Ruleset


Hackathon (example)

Developer
Designer
Producer

Color each type a single color (i.e. Developer Magenta, Producer Yellow, Designer Cyan)
As the color becomes brighter or pulses slower, the person is happy or successful.
As conditions worsen, the pulse rate speeds up or the light dims

Rulesets
Developers +
	>1 developer
	<5 developer
	>=1 designer
	<3 designer
	==1 producer

Designers +
	developer <= 1.5*designer
	==1 producer

Producers +
	<= 2 producer
	>=1 developer
	>=1 designer
