<script lang="ts">
    import {Button} from "$lib/components/ui/button";
    import { allLightsOn, allLightsOff } from "$lib/gala";
    import type { PageData } from './$types';

    let { data }: { data: PageData } = $props();
</script>

<h1>DALI/Gala</h1>

<div class="mb-6">
    <Button onclick={allLightsOn} class="cursor-pointer mr-2">All Lights On</Button>
    <Button onclick={allLightsOff} class="cursor-pointer">All Lights Off</Button>
</div>

{#if data.error}
    <div class="text-center py-4 text-red-600">
        <p>Error: {data.error}</p>
    </div>
{:else}
    <div class="space-y-4">
        <h2 class="text-xl font-semibold">Device Statuses</h2>
        {#if data.deviceStatuses.length === 0}
            <p class="text-gray-500">No devices found</p>
        {:else}
            <div class="grid gap-4">
                {#each data.deviceStatuses as device}
                    <div class="border rounded-lg p-4 bg-gray-50">
                        <h3 class="font-medium">Device {device.shortAddress}</h3>
                        <div class="mt-2 space-y-1 text-sm">
                            <div class="flex justify-between">
                                <span>Port 1 Connected:</span>
                                <span class={device.status.port1Connected ? 'text-green-600' : 'text-red-600'}>
                                    {device.status.port1Connected ? 'Yes' : 'No'}
                                </span>
                            </div>
                            <div class="flex justify-between">
                                <span>Port 2 Connected:</span>
                                <span class={device.status.port2Connected ? 'text-green-600' : 'text-red-600'}>
                                    {device.status.port2Connected ? 'Yes' : 'No'}
                                </span>
                            </div>
                            <div class="flex justify-between">
                                <span>Any Port On:</span>
                                <span class={device.status.anyPortOn ? 'text-green-600' : 'text-red-600'}>
                                    {device.status.anyPortOn ? 'Yes' : 'No'}
                                </span>
                            </div>
                            <div class="flex justify-between">
                                <span>Arc Out of Range:</span>
                                <span class={device.status.arcOutOfRange ? 'text-red-600' : 'text-green-600'}>
                                    {device.status.arcOutOfRange ? 'Yes' : 'No'}
                                </span>
                            </div>
                            <div class="flex justify-between">
                                <span>Action in Progress:</span>
                                <span class={device.status.actionInProgress ? 'text-yellow-600' : 'text-green-600'}>
                                    {device.status.actionInProgress ? 'Yes' : 'No'}
                                </span>
                            </div>
                            <div class="flex justify-between">
                                <span>Device Not Configured:</span>
                                <span class={device.status.deviceNotConfigured ? 'text-red-600' : 'text-green-600'}>
                                    {device.status.deviceNotConfigured ? 'Yes' : 'No'}
                                </span>
                            </div>
                            <div class="flex justify-between">
                                <span>Missing Short Address:</span>
                                <span class={device.status.missingShortAddress ? 'text-red-600' : 'text-green-600'}>
                                    {device.status.missingShortAddress ? 'Yes' : 'No'}
                                </span>
                            </div>
                            <div class="flex justify-between">
                                <span>Missing Arc Level:</span>
                                <span class={device.status.missingArcLevel ? 'text-red-600' : 'text-green-600'}>
                                    {device.status.missingArcLevel ? 'Yes' : 'No'}
                                </span>
                            </div>
                        </div>
                    </div>
                {/each}
            </div>
        {/if}
    </div>
{/if}