import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';

export const GET: RequestHandler = async ({ params }) => {
	// Only return mock data in development
	if (process.env.NODE_ENV !== 'development') {
		return json({ error: 'Not available in production' }, { status: 404 });
	}

	const address = parseInt(params.address || '0');
	// Return mock status: address 0 gets 135, others get -101
	const mockStatus = address === 0 ? 135 : -101;

	return json({ status: mockStatus });
};
