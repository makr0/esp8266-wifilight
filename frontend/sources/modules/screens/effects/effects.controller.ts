module app {

  'use strict';

  /**
   * Select effects on this screen
   */
  export class EffectsController {

    effects: Array<string>;
    loading: boolean;

    private logger: ILogger;
    private esplightService: EsplightService;

    constructor(logger: LoggerService,
                esplightService: EsplightService) {

      this.logger = logger.getLogger('effects');
      this.logger.log('init');
      this.esplightService = esplightService;

      this.refreshList();

    }

    refreshList() {
      this.loading = true;
      this.esplightService.getEffects()
      .then((effects: Array<string>) => {
        this.effects = effects;
      })
      .finally(() => {
        this.loading = false;
      });
    }

    setEffect( effect: string ) {
      this.esplightService.setEffect(effect);
    }

  }

  angular
    .module('app')
    .controller('effectsController', EffectsController);

}
