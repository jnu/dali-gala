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
    portOneConnected: boolean;
    portTwoConnected: boolean;
    arcPowerOn: boolean;
    arcLevelOutOfRange: boolean;
    fadeInProgress: boolean;
    deviceNotConfigured: boolean;
    shortAddressMissing: boolean;
    arcLevelMissing: boolean;
}

/**
 * Error status information.
 */
export type DALIErrorStatus = {
    error: string;
    errorCode: number;
}

/**
 * Device information.
 */
export type Device = {
    shortAddress: number;
    status: DALIStatus
    error: DALIErrorStatus | null;
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
 * Set a specific light to a specific level.
 * 
 * @param address 
 * @param level 
 */
export const setLightLevel = async (address: number, level: number) => {
    await fetch('/api/v1/devices', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ state: level > 0, addr: address, level }),
    });
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
 * Execute a DALI command.
 * 
 * @param cmd 
 * @param arg 
 * @returns 
 */
export const executeCommand = async (cmd: number, arg: number): Promise<number> => {
    const response = await fetch('/api/v1/cmd', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ cmd, arg }),
    });
    const json = await response.json() as { result: number } | GalaAPIErrorResponse;
    if ('error' in json) {
        throw new Error(json.error);
    }
    return json.result;
}

/**
 * Set the DTR of a device.
 * 
 * @param dtr 
 * @param address 
 * @param value 
 */
export const setDTR = async (dtr: number, address: number, value: number) => {
    await fetch('/api/v1/dtr', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ dtr, addr: address, value }),
    });
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
            error: expandError(status),
        };
    });
}

/**
 * Look up error codes and add a readable description.
 * 
 * @param status 
 * @returns 
 */
const expandError = (status: number): DALIErrorStatus | null => {
    switch (status) {
        case -101:
            return {
                error: 'No reply from device',
                errorCode: status,
            };
        default:
            if (status < 0) {
                return {
                    error: 'Unknown error',
                    errorCode: status,
                };
            }
            return null;
    }
}

/**
 * Some examples of statuses:
 * 
 * ERRORS
 * -------
 * Response < 0 is an error code.
 * 
 *       Int: -101
 *   Meaning: No reply from device: -101
 * 
 * STATUSES
 * --------
 * Responses >= 0 contain granular status information.
 * 
 * Scenario: Ballast is healthy and OFF
 *      Int: 131
 *   Binary: 10000011
 * Expanded:
 *    bit 0 / portOneConnected: true
 *    bit 1 / portTwoConnected: true
 *    bit 2 / arcPowerOn: false
 *    bit 3 / arcLevelOutOfRange: false
 *    bit 4 / fadeInProgress: false
 *    bit 5 / deviceNotConfigured: false
 *    bit 6 / shortAddressMissing: false
 *    bit 7 / arcLevelMissing: true
 * 
 * Scenario: Ballast is healthy and ON
 *      Int: 7
 *   Binary: 00000111
 * Expanded:
 *    bit 0 / portOneConnected: true
 *    bit 1 / portTwoConnected: true
 *    bit 2 / arcPowerOn: true
 *    bit 3 / arcLevelOutOfRange: false
 *    bit 4 / fadeInProgress: false
 *    bit 5 / deviceNotConfigured: false
 *    bit 6 / shortAddressMissing: false
 *    bit 7 / arcLevelMissing: false
 */

/**
 * Unpack the binary status data into a more interpretable object.
 * 
 * @param status 
 * @returns 
 */
const expandStatus = (status: number): DALIStatus => {
    const keys = [
        'portOneConnected',
        'portTwoConnected',
        'arcPowerOn',
        'arcLevelOutOfRange',
        'fadeInProgress',
        'deviceNotConfigured',
        'shortAddressMissing',
        'arcLevelMissing',
    ] as const;

    // Status codes <0 are errors, so don't bother trying to interpret them.
    if (status < 0) {
        return keys.reduce((acc, key) => {
            acc[key] = false;
            return acc;
        }, {} as DALIStatus);
    }

    return keys.reduce((acc, key) => {
        const bit = status & 0x01;
        acc[key] = !!bit;
        status >>= 1;
        return acc;
    }, {} as DALIStatus);
}