module app {

  'use strict';

  /**
   * EsplightService: control a ESp-wifi light
   */
  export class EsplightService {

    private ROUTES = {
      listEffects: '/effects',
      setEffect: '/effect',
      status: '/status',
      setColor: '/colors',
      getStatus: '/status'
    };
    private baseUrl: string = '';
    private logger: ILogger;

    constructor( private $q: ng.IQService,
                 private $http: ng.IHttpService,
                 logger: LoggerService ) {
      this.logger = logger.getLogger('esplightService');
    };

    /**
     * Get list of effects on the Light
     * @return Array of {Strings}
     */
    getEffects(): ng.IPromise<Array<string>> {
      return this.$http.get(this.baseUrl + this.ROUTES.listEffects)
        .then((response: any ) => {
          this.logger.log(response);
          return response.data.effects;
        })
        .catch(function() {
          return [ 'Error, could not load effects' ];
        });
    };

    setEffect(effect: string): ng.IPromise<any> {
      this.logger.log('setting effect' + effect);
      return this.$http.get(this.baseUrl + this.ROUTES.setEffect, {params: {e: effect}})
        .then((response: any ) => {
          this.logger.log(response);
          return response.data.message;
        })
        .catch(function() {
          return [ 'Error, could not set effect' ];
        });
    };

    getStatus(): ng.IPromise<any> {
      return this.$http.get(this.baseUrl + this.ROUTES.getStatus)
        .then((response: any ) => {
          this.logger.log(response);
          return response.data;
        })
        .catch(function() {
          return [ 'Error, could not read status' ];
        });
    };

    setColor(color: IColor): ng.IPromise<any> {
      this.logger.log('setting color' + color.H + color.S + color.L);
      let params = {
        index: 0,
        h: color.H,
        s: color.S,
        l: color.L
      };
      return this.$http.get(this.baseUrl + this.ROUTES.setColor, {params: params} )
        .then((response: any ) => {
          this.logger.log(response);
          return response.data.message;
        })
        .catch(function() {
          return [ 'Error, could not set effect' ];
        });
    };

    setServer(server: string): void {
      this.baseUrl = server;
    }

  }

  angular
    .module('app')
    .service('esplightService', EsplightService);

}
