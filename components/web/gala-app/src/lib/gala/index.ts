export const allLightsOn = async () => {
    await fetch('/api/v1/devices', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            state: true,            
        })
    });
}

export const allLightsOff = async () => {
    await fetch('/api/v1/devices', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            state: false,
        })
    });
}