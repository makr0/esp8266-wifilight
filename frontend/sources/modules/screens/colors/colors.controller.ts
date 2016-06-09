module app {

  'use strict';
  export interface IColor {
    H: number;
    S: number;
    L: number;
  }

  export class ColorsController {

    private color: IColor;
    private logger: ILogger;
    private colorIndex: number;

    constructor(logger: LoggerService,
                $scope: ng.IScope,
                esplightService: EsplightService) {

      this.logger = logger.getLogger('color');
      this.logger.log('init');
      this.color = { H: 0, S: 0, L: 0 };
      this.colorIndex = 0;

      $scope.$watch(() => this.color, (newValue, oldValue) => {
        if (oldValue !== newValue) {
          // color has changed
          this.logger.log('color has changed');
          if ( !angular.isUndefined( newValue ) ) {
            esplightService.setColor(newValue);
          }
        }
      }, true);
    }
  }

  angular
    .module('app')
    .controller('colorsController', ColorsController);

}
