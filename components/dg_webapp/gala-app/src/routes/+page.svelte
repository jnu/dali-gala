<script lang="ts">
    import {Button} from "$lib/components/ui/button";
    import { allLightsOn, allLightsOff } from "$lib/gala";
</script>

<h1>DALI/Gala</h1>
<div>
    <Button onclick={allLightsOn} class="cursor-pointer">All Lights On</Button>
    <Button onclick={allLightsOff} class="cursor-pointer">All Lights Off</Button>
</div>