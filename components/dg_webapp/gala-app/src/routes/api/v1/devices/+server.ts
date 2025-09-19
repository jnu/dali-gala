import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';

// Mock data: first number is 135, rest are -101
const mockStatuses = [135, ...Array(63).fill(-101)];

export const GET: RequestHandler = async () => {
	// Only return mock data in development
	if (process.env.NODE_ENV !== 'development') {
		return json({ error: 'Not available in production' }, { status: 404 });
	}

	return json({ statuses: mockStatuses });
};

export const POST: RequestHandler = async ({ request }) => {
	// Only handle in development
	if (process.env.NODE_ENV !== 'development') {
		return json({ error: 'Not available in production' }, { status: 404 });
	}

	try {
		const body = await request.json();
		console.log('Mock: Toggle all lights to', body.state ? 'ON' : 'OFF');
		return json({ success: true });
	} catch (error) {
		return json({ error: 'Invalid JSON' }, { status: 400 });
	}
};
