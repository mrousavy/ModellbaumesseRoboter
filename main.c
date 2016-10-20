void afterSpeedChange(int lastCase, int speed) {
	int switch;
	if (lastCase != -1) {
		switch = lastCase;
	} else {
		switch = remote_read();
	}
	switch (switch) {
		case -1:
			return 0;
			break;
		case 0:
			motor(0, 0);
			motor(2, 0);
			break;
		case 1:
			motor(0, speed);
			motor(2, -speed);
			if (lastCase != -1) {
				lastCase = -1;
			}
			break;
		case 2:
			motor(0, -speed);
			motor(2, speed);
			lastCase = -1;
			break;
		case 3:
			motor(0, -speed);
			motor(2, -speed);
			lastCase = -1;
			break;
		case 4:
			motor(0, speed);
			motor(2, speed);
			lastCase = -1;
			break;
		case 5:
			if (speed > 90) {
				printf("Maximum Speed reached");
			} else {
				speed += 10;
				afterSpeedChange(lastCase, speed);
			}
			break;
		case 6:
			if (speed < 10) {
				printf("Minimum Speed reached");
			} else {
				speed -= 10;
				afterSpeedChange(lastCase, speed);
			}
			break;
	}
}

int main() {
	remote_setup();
	printf("Server setted up!");
	int speed = 20;
	int lastCase;
	enable_servo(0);
	while (1) {
		move(lastCase, speed);
	}
	return 0;
}
