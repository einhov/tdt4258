struct saw_voice {
	float phase;
	float freq;
};

struct square_voice {
	float phase;
	int freq;
};
struct sine_voice {
	float phase;
	float freq;
};

struct triangle_voice {
	float phase;
	float freq;
	bool up;
};

uint32_t saw_wave(struct saw_voice *v);
uint32_t square_wave(struct square_voice *v);
uint32_t sine_wave(struct sine_voice *v);
uint32_t triangle_wave(struct triangle_voice *v);
