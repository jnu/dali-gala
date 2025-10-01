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
                        <div class="text-xs font-medium text-gray-700 p-1">{device.shortAddress}</div>
                        <div class="text-center px-2 pb-2">
                            
                            {#if device.status.port1Connected || device.status.port2Connected}
                                <div class="mt-1 flex flex-wrap justify-center gap-1">
                                    <!-- Status indicators as small badges - only show when device is connected -->
                                    <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium {device.status.port1Connected ? 'bg-green-100 text-green-800' : 'bg-red-100 text-red-800'}">
                                        P1
                                    </span>
                                    <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium {device.status.port2Connected ? 'bg-green-100 text-green-800' : 'bg-red-100 text-red-800'}">
                                        P2
                                    </span>
                                    <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium {device.status.anyPortOn ? 'bg-green-100 text-green-800' : 'bg-gray-100 text-gray-800'}">
                                        ON
                                    </span>
                                    {#if device.status.arcOutOfRange}
                                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">
                                            ARC
                                        </span>
                                    {/if}
                                    {#if device.status.actionInProgress}
                                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-yellow-100 text-yellow-800">
                                            ACT
                                        </span>
                                    {/if}
                                    {#if device.status.deviceNotConfigured}
                                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">
                                            CFG
                                        </span>
                                    {/if}
                                    {#if device.status.missingShortAddress}
                                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">
                                            ADDR
                                        </span>
                                    {/if}
                                    {#if device.status.missingArcLevel}
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
                        <span class="inline-flex items-center px-1.5 py-0.5 rounded text-xs font-medium bg-red-100 text-red-800">ARC</span>
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