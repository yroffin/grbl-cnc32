import { Injectable } from '@angular/core';
import { HttpClient, HttpErrorResponse } from '@angular/common/http';
import { throwError, Observable } from 'rxjs';
import { catchError, retry } from 'rxjs/operators';
import { Status } from '../models/grbl';

export interface Config {
  heroesUrl: string;
  textfile: string;
}

@Injectable({
  providedIn: 'root'
})
export class GrblService {

  configUrl: string;

  constructor(private http: HttpClient) {
    this.configUrl = '/api/';
  }

  getStatus(): Observable<Status> {
    return this.http.get<Status>(this.configUrl + 'v1/simulate')
      .pipe(
        retry(0), // retry a failed request up to 3 times
        catchError(this.handleError) // then handle the error
      );
  }

  setStatus(body: string): Observable<Status> {
    console.info('set', body);
    return this.http.put<Status>(this.configUrl + 'v1/simulate', body)
      .pipe(
        retry(0), // retry a failed request up to 3 times
        catchError(this.handleError) // then handle the error
      );
  }

  private handleError(error: HttpErrorResponse) {
    if (error.error instanceof ErrorEvent) {
      // A client-side or network error occurred. Handle it accordingly.
      console.error('An error occurred:', error.error.message);
    } else {
      // The backend returned an unsuccessful response code.
      // The response body may contain clues as to what went wrong,
      console.error(
        `Backend returned code ${error.status}, ` +
        `body was: ${error.error}`);
    }
    // return an observable with a user-facing error message
    return throwError(
      'Node bad happened; please try again later.');
  }
}
