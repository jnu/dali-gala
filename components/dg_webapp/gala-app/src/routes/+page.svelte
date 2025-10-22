<script lang="ts">
    import {Button} from "$lib/components/ui/button";
    import { Slider } from "$lib/components/ui/slider";
    import * as gala from "$lib/gala";
    import type { PageData } from './$types';

    let { data }: { data: PageData } = $props();

    if (typeof window !== 'undefined') {
        // @ts-ignore
        window['gala'] = gala;
    }
</script>

<h1>DALI/Gala</h1>

<div class="mb-6">
    <Button onclick={gala.allLightsOn} class="cursor-pointer mr-2">All Lights On</Button>
    <Button onclick={gala.allLightsOff} class="cursor-pointer">All Lights Off</Button>
</div>

{#if data.error}
    <div class="text-center py-4 text-red-600">
        <p>Error: {data.error}</p>
    </div>
{:else}
    <div class="space-y-4 m-4">
        <div class="flex justify-between items-center">
            <h2 class="text-xl font-semibold">Device Statuses</h2>
            <div class="text-sm text-gray-500">
                {data.deviceStatuses.length} devices
            </div>
        </div>
        {#if data.deviceStatuses.length === 0}
            <p class="text-gray-500">No devices found</p>
        {:else}
            <!-- Compact grid layout -->
            <div class="grid grid-cols-2 sm:grid-cols-4 md:grid-cols-6 lg:grid-cols-8 m-4">
                {#each data.deviceStatuses as device}
                    <div class="border bg-gray-50 hover:bg-gray-100 transition-colors">
                        {#if device.error}
                            <div class="text-xs font-medium text-gray-400 p-1">{device.shortAddress}</div>
                        {:else}
                            <div class="text-xs font-medium text-gray-700 p-1">{device.shortAddress}</div>
                        {/if}
                        <div class="text-center px-2 pb-2">
                            {#if device.error}
                                <div class="mt-1 text-xs text-gray-400">
                                    {device.error.error}
                                </div>
                            {:else}
                            {#if device.status.portOneConnected || device.status.portTwoConnected}
                                <div class="mt-1 flex flex-wrap justify-center gap-1">
                                    <div class="w-full my-2">
                                        <Slider type="single" min={0} max={254} step={1} onValueCommit={(value: number) => gala.setLightLevel(device.shortAddress, value)} />
                                    </div>
                                    <div class="w-full my-2">
                                        <Slider type="single" min={0} max={1000} step={1} onValueCommit={(value: number) => gala.setCCT(device.shortAddress, value)} />
                                    </div>
                                    <!-- Status indicators as small badges - only show when device is connected -->
                                    <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium {device.status.portOneConnected ? 'bg-green-100 text-green-800' : 'bg-red-100 text-red-800'}">
                                        P1
                                    </span>
                                    <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium {device.status.portTwoConnected ? 'bg-green-100 text-green-800' : 'bg-red-100 text-red-800'}">
                                        P2
                                    </span>
                                    <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium {device.status.arcPowerOn ? 'bg-green-100 text-green-800' : 'bg-gray-100 text-gray-800'}">
                                        ON
                                    </span>
                                    {#if device.status.arcLevelOutOfRange}
                                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">
                                            ARC
                                        </span>
                                    {/if}
                                    {#if device.status.fadeInProgress}
                                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-yellow-100 text-yellow-800">
                                            ACT
                                        </span>
                                    {/if}
                                    {#if device.status.deviceNotConfigured}
                                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">
                                            CFG
                                        </span>
                                    {/if}
                                    {#if device.status.shortAddressMissing}
                                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">
                                            ADDR
                                        </span>
                                    {/if}
                                    {#if device.status.arcLevelMissing}
                                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">
                                            LEVEL
                                        </span>
                                    {/if}
                                </div>
                            {:else}
                                <div class="mt-1 text-xs text-gray-400">
                                    Not Connected
                                </div>
                            {/if}
                            {/if}
                        </div>
                    </div>
                {/each}
            </div>
            
            <!-- Legend -->
            <div class="mt-4 p-3 bg-gray-100 rounded-lg">
                <h3 class="text-sm font-medium text-gray-700 mb-2">Status Legend:</h3>
                <div class="text-xs text-gray-600 mb-3">
                    <strong>Note:</strong> Status indicators only appear for connected devices (at least one port connected).
                </div>
                <div class="grid grid-cols-2 sm:grid-cols-4 gap-2 text-xs">
                    <div class="flex items-center gap-1">
                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-green-100 text-green-800">P1</span>
                        <span>Port 1 Connected</span>
                    </div>
                    <div class="flex items-center gap-1">
                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-green-100 text-green-800">P2</span>
                        <span>Port 2 Connected</span>
                    </div>
                    <div class="flex items-center gap-1">
                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-green-100 text-green-800">ON</span>
                        <span>Any Port On</span>
                    </div>
                    <div class="flex items-center gap-1">
                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">LVL</span>
                        <span>Arc Out of Range</span>
                    </div>
                    <div class="flex items-center gap-1">
                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-yellow-100 text-yellow-800">ACT</span>
                        <span>Action in Progress</span>
                    </div>
                    <div class="flex items-center gap-1">
                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">CFG</span>
                        <span>Not Configured</span>
                    </div>
                    <div class="flex items-center gap-1">
                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">ADDR</span>
                        <span>Missing Address</span>
                    </div>
                    <div class="flex items-center gap-1">
                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">LEVEL</span>
                        <span>Missing Arc Level</span>
                    </div>
                </div>
            </div>
        {/if}
    </div>
{/if}