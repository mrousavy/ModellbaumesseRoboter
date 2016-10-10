void afterSpeedChange(int lastCase, int speed) {
	switch (lastCase) {
		case 1:
			motor(0, speed);
			motor(1, speed);
			motor(2, -speed);
			motor(3, -speed);
			break;
		case 2:
			motor(0, -speed);
			motor(1, -speed);
			motor(2, speed);
			motor(3, speed);
			break;
		case 3:
			motor(0, -speed);
			motor(1, -speed);
			motor(2, -speed);
			motor(3, -speed);
			break;
		case 4:
			motor(0, speed);
			motor(1, speed);
			motor(2, speed);
			motor(3, speed);
			break;
	}
}

int main() {
	remote_setup();
	printf("Server setted up!");
	int speed = 20;
	int lastCase;
	while (1) {
		switch (remote_read()) {
			case -1:
				return 0;
				break;
			case 0:
				motor(0, 0);
				motor(1, 0);
				motor(2, 0);
				motor(3, 0);
				break;
			case 1:
				motor(0, speed);
				motor(1, speed);
				motor(2, -speed);
				motor(3, -speed);
				lastCase = 1;
				break;
			case 2:
				motor(0, -speed);
				motor(1, -speed);
				motor(2, speed);
				motor(3, speed);
				lastCase = 2;
				break;
			case 3:
				motor(0, -speed);
				motor(1, -speed);
				motor(2, -speed);
				motor(3, -speed);
				lastCase = 3;
				break;
			case 4:
				motor(0, speed);
				motor(1, speed);
				motor(2, speed);
				motor(3, speed);
				lastCase = 4;
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
	return 0;
}
