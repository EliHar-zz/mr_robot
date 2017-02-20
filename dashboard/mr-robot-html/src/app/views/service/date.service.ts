import { Injectable } from '@angular/core';

@Injectable()
export class DateService {

  private dateTime : Date;

  constructor() {
    // Every 1 second
    this.dateTime = new Date();
    setInterval(()=>{
      this.dateTime = new Date();
    },1000);
  }

  public get getDate() {
    return this.dateTime;
  }
}
