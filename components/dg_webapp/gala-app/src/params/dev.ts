import { dev } from '$app/environment';
import type { ParamMatcher } from '@sveltejs/kit';

// Routes that only exist in development mode.
const DEV_ROUTES = [
    "api"
];

/**
 * Match the parameter to a route that only exists in development mode.
 * 
 * @param param - The parameter to match.
 * @returns 
 */
export const match: ParamMatcher = (param) => {
    return dev && DEV_ROUTES.includes(param);
}