/**
 * API error response.
 */
export type GalaAPIErrorResponse = {
    error: string;
    code: number;
}

/**
 * API response for a list of device statuses.
 */
export type GalaAPIResponseStatuses = {
    statuses: number[];
}

/**
 * API response for a single device status.
 */
export type GalaAPIResponseStatus = {
    status: number;
}

/**
 * Device status information.
 */
export type DALIStatus = {
    port1Connected: boolean;
    port2Connected: boolean;
    anyPortOn: boolean;
    arcOutOfRange: boolean;
    actionInProgress: boolean;
    deviceNotConfigured: boolean;
    missingShortAddress: boolean;
    missingArcLevel: boolean;
}

/**
 * Device information.
 */
export type Device = {
    shortAddress: number;
    status: DALIStatus;
};

/**
 * Toggle all lights on or off.
 * 
 * @param state - True to turn on, false to turn off.
 * @param state 
 */
const toggleAllLights = async (state: boolean) => {
    await fetch('/api/v1/devices', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ state }),
    });
}

/**
 * Broadcast all lights to on.
 */
export const allLightsOn = async () => {
    await toggleAllLights(true);
}

/**
 * Broadcast all lights to off.
 */
export const allLightsOff = async () => {
    await toggleAllLights(false);
}

/**
 * Get the status of a device.
 * 
 * @param address - The short address of the device to get the status of.
 * @returns The status of the device.
 */
export const getDeviceStatus = async (address: number): Promise<Device> => {
    const response = await fetch(`/api/v1/devices/${address}`);
    const json = await response.json() as GalaAPIResponseStatus | GalaAPIErrorResponse;
    if ('error' in json) {
        throw new Error(json.error);
    }
    const status = expandStatus(json.status);
    return {
        shortAddress: address,
        status,
    };
}

/**
 * Get all device statuses.
 * 
 * @returns List of all device statuses.
 */
export const getAllDeviceStatuses = async (): Promise<Device[]> => {
    const response = await fetch('/api/v1/devices');
    const json = await response.json() as GalaAPIResponseStatuses | GalaAPIErrorResponse;
    if ('error' in json) {
        throw new Error(json.error);
    }
    return json.statuses.map((status, i) => {
        return {
            shortAddress: i,
            status: expandStatus(status),
        };
    });
}

/**
 * Unpack the binary status data into a more interpretable object.
 * 
 * @param status 
 * @returns 
 */
const expandStatus = (status: number) => {
    const keys = [
        'port1Connected',
        'port2Connected',
        'anyPortOn',
        'arcOutOfRange',
        'actionInProgress',
        'deviceNotConfigured',
        'missingShortAddress',
        'missingArcLevel',
    ] as const;

    return keys.reduce((acc, key) => {
        const bit = status & 0x01;
        acc[key] = !!bit;
        status >>= 1;
        return acc;
    }, {} as DALIStatus);
}