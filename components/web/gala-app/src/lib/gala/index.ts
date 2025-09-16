export const allLightsOn = async () => {
    await fetch('/api/v1/lights', {
        method: 'POST',
        body: JSON.stringify({
            state: true,
        })
    });
}

export const allLightsOff = async () => {
    await fetch('/api/v1/lights', {
        method: 'POST',
        body: JSON.stringify({
            state: false,
        })
    });
}