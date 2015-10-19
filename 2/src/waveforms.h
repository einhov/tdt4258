struct saw_voice {
	uint32_t phase;
	uint32_t freq;
};

struct square_voice {
	uint32_t phase;
	uint32_t freq;
};

struct triangle_voice {
	int32_t phase;
	uint32_t freq;
	bool up;
};

uint32_t saw_wave(struct saw_voice *v);
uint32_t square_wave(struct square_voice *v);
uint32_t triangle_wave(struct triangle_voice *v);
