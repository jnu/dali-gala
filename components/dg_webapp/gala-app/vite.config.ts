import tailwindcss from '@tailwindcss/vite';
import { sveltekit } from '@sveltejs/kit/vite';
import { defineConfig } from 'vite';

export default defineConfig({
	plugins: [tailwindcss(), sveltekit()],
	build: {
		rollupOptions: {
			// Exclude the API routes from the build,
			// since the real API is in the ESP32 firmware.
			external: ["src/routes/api"]
		}
	}
});
