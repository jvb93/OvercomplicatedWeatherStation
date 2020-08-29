<template>
  <article class="tile is-parent is-vertical">
    <nav class="level">
      <div class="level-left">
        <div class="level-item">
          <a
            class="button is-light is-outlined is-small"
            href="https://github.com/jvb93/OvercomplicatedWeatherStation"
            target="_blank"
            >Made by @jvb93
          </a>
        </div>
      </div>
      <div class="level-right">
        <p class="level-item">
          <b-switch v-model="useImperial" size="is-small" class="has-text-light"
            >imperial units</b-switch
          >
        </p>
      </div>
    </nav>
  </article>
</template>

<script>
export default {
  computed: {
    useImperial: {
      // getter
      get: function() {
        return localStorage["useImperial"];
      },
      // setter
      set: function(newValue) {
        if (newValue) {
          localStorage["useImperial"] = true;
        } else {
          localStorage["useImperial"] = false;
        }

        location.reload();
      },
    },
  },
};
</script>
