import { getAllDeviceStatuses, type Device } from '$lib/gala';
import type { PageLoad } from './$types';

export const load: PageLoad = async () => {
    try {
        const deviceStatuses = await getAllDeviceStatuses();
        return {
            deviceStatuses,
            error: null
        };
    } catch (error) {
        console.error('Error loading device statuses:', error);
        return {
            deviceStatuses: [],
            error: error instanceof Error ? error.message : 'Failed to load device statuses'
        };
    }
};
